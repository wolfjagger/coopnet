#include "dpll_solver.h"
#include <iostream>
#include "boost/logic/tribool.hpp"
#include "coopnet/sat/problem/problem.h"
#include "dpll_formula.h"
#include "dpll_node_chooser.h"

using namespace coopnet;



namespace {

	constexpr bool DEBUG = false;

	void DEBUG_print_assignment(const DPLLFormula& formula) {
		if (DEBUG) {
			auto print_pred =
				[](IncompleteAssignment::value_type pair) {
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
			auto& assign = formula.create_incomplete_assignment();
			std::for_each(assign.cbegin(), assign.cend(), print_pred);
			std::cout << "\n";
		}
	}

}



DPLLSolver::DPLLSolver(DPLLNodeChoiceMode mode) :
	nodeChooser(create_dpll_node_chooser(mode)) {

}

DPLLSolver::~DPLLSolver() {

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

auto DPLLSolver::do_solve(const Problem& prob) -> Solution {
	
	if (DEBUG) {
		std::cout << "Solving problem:\n";
		std::cout << prob;
	}

	formula = std::make_unique<DPLLFormula>(prob);
	decisions = std::stack<DPLLNodeChoiceBranch>();

	find_assignment();

	Solution solution;
	if (formula->is_SAT()) {
		auto assign = Assignment(formula->create_assignment());
		solution = Solution{
			SolutionStatus::Satisfied,
			std::make_shared<coopnet::Assignment>(std::move(assign)),
			0
		};
	} else {
		solution = Solution{
			SolutionStatus::Unsatisfied,
			nullptr,
			0
		};
	}

	formula = nullptr;
	return solution;

}



void DPLLSolver::find_assignment() {

	while (true) {

		auto new_choice = nodeChooser->choose(*formula);

		// If no valid new node, formula is reduced
		if (!new_choice.is_initialized()) break;

		auto decision = DPLLNodeChoiceBranch{ new_choice.get(), true };
		reduce_with_selection(decision);
		
		if (formula->is_contradicting()) {

			auto last_choice_success = change_last_free_choice();

			if (!last_choice_success) break;

		}

	}

	DEBUG_print_assignment(*formula);

}



bool DPLLSolver::change_last_free_choice() {

	if (DEBUG) std::cout << "Contradictory\n";

	while(!decisions.empty()) {

		auto decision = decisions.top();
		decisions.pop();

		auto choice = decision.choice;
		formula->reverse_prune_to_assignment(choice.n);
		formula->set_uncontradicting();
		DEBUG_print_assignment(*formula);

		if (decision.is_first_choice) {

			auto new_choice = DPLLNodeChoice{ choice.n, !choice.sgn };
			auto new_decision = DPLLNodeChoiceBranch{ new_choice, false };
			reduce_with_selection(new_decision);
			if(!formula->is_contradicting()) return true;

		}

	}

	formula->set_contradicting();
	return false;

}

void DPLLSolver::reduce_with_selection(DPLLNodeChoiceBranch decision) {

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
