#include "dpll_solver.h"
#include <iostream>
#include "boost/logic/tribool.hpp"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/component/node.h"
#include "dpll_formula.h"
#include "dpll_node_chooser.h"

using namespace sat;



namespace {

	constexpr bool DEBUG = false;

	void DEBUG_print_assignment(const dpll_formula& formula) {
		if(DEBUG) {
			auto tmp_pred =
				[](incomplete_assignment::pair pair) {
				if (pair.second) {
					std::cout << "T";
				} else if (!pair.second) {
					std::cout << "F";
				} else if (boost::logic::indeterminate(pair.second)) {
					std::cout << "I";
				} else {
					std::cout << "O";
				}
			};
			auto& assign = formula.get_incomplete_assignment();
			std::for_each(assign.data.cbegin(), assign.data.cend(), tmp_pred);
			std::cout << "\n";
		}
	}

}



dpll_solver::dpll_solver(dpll_node_choice_mode mode) :
	complete_solver(std::move(create_dpll_node_chooser(mode))) {

}

dpll_solver::~dpll_solver() {

}



/*
* Recursive DPLL(F,p)
* Input : A CNF formula F and an initially empty partial assignment p
* Output : UNSAT, or an assignment satisfying F
*  begin
*   (F,p) <- UnitPropagate(F,p)
*   if F contains the empty clause then return UNSAT
*   if F has no clauses left then
*    Output p
*    return SAT
*   x <- a literal not assigned by p // the branching step
*   if DPLL-recursive(F|x, p || {x}) = SAT then return SAT
*   return DPLL-recursive(F|-x, p || {-x})
*  end
*
* sub UnitPropagate(F,p)
*  begin
*   while F contains no empty clause but has a unit clause u do
*    F <- F|u
*    p <- p || {u}
*   return (F,p)
*  end
*/

auto dpll_solver::do_solve(const problem& prob) -> solve_return {
	
	if (DEBUG) {
		std::cout << "Solving problem:\n";
		std::cout << prob;
	}

	formula = std::make_unique<dpll_formula>(prob);
	decisions = std::stack<node_decision>();

	find_assignment();

	if (formula->is_SAT()) {
		auto assign = assignment(formula->get_incomplete_assignment());
		return std::make_pair(
			solution_status::Satisfied,
			std::make_shared<sat::assignment>(std::move(assign)));
	} else {
		return std::make_pair(
			solution_status::Unsatisfiable, std::shared_ptr<sat::assignment>());
	}

	formula = nullptr;

}



void dpll_solver::find_assignment() {

	while (true) {

		auto new_choice = n_chooser->choose(*formula);

		// If no valid new node, formula is reduced
		if (!new_choice.is_initialized()) break;

		auto decision = node_decision{ new_choice.get(), true };
		reduce_with_selection(decision);
		
		if (formula->is_contradicting()) {

			auto last_choice_success = change_last_free_choice();

			if (!last_choice_success) break;

		}

	}

	DEBUG_print_assignment(*formula);

}



bool dpll_solver::change_last_free_choice() {

	if (DEBUG) std::cout << "Contradictory\n";

	while(!decisions.empty()) {

		auto decision = decisions.top();
		decisions.pop();

		auto choice = decision.choice;
		formula->reverse_prune_to_assignment(choice.n);
		formula->set_contradicting(false);
		DEBUG_print_assignment(*formula);

		if (decision.is_first_choice) {

			auto new_choice = node_choice{ choice.n, !choice.sgn };
			auto new_decision = node_decision{ new_choice, false };
			reduce_with_selection(new_decision);
			if(!formula->is_contradicting()) return true;

		}

	}

	formula->set_contradicting(true);
	return false;

}

void dpll_solver::reduce_with_selection(node_decision decision) {

	if (DEBUG) {
		std::cout << "Choose node " << std::to_string(decision.choice.n.id);
		std::cout << " to be " << (decision.choice.sgn ? "true" : "false");
		std::cout << " for the " << (decision.is_first_choice ? "first" : "second");
		std::cout << " time." << std::endl;
	}

	decisions.push(decision);
	formula->set_node(decision.choice);
	DEBUG_print_assignment(*formula);

}
