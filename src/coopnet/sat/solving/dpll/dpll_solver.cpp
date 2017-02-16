#include "dpll_solver.h"
#include <iostream>
#include "boost/logic/tribool.hpp"
#include "alphali/containers/random_iterator.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/component/node.h"

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
	choices = std::stack<std::pair<node, bool>>();

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

		auto new_node = choose_next_node(node_choice_mode::Next);

		// If no valid new node, formula is reduced
		if (!new_node.is_initialized()) break;

		reduce_with_selection(*new_node, true);
		
		if (formula->is_contradicting()) {

			auto last_choice_success = change_last_free_choice();

			if (!last_choice_success) break;

		}

	}

	DEBUG_print_assignment(*formula);

}



bool dpll_solver::change_last_free_choice() {

	if (DEBUG) std::cout << "Contradictory\n";

	while(!choices.empty()) {

		auto choice = choices.top();
		choices.pop();

		auto n = choice.first;
		auto b = choice.second;
		formula->reverse_prune_to_assignment(n);
		formula->set_contradicting(false);
		DEBUG_print_assignment(*formula);

		if (b == true) {

			reduce_with_selection(n, !b);
			if(!formula->is_contradicting()) return true;

		}

	}

	formula->set_contradicting(true);
	return false;

}

void dpll_solver::reduce_with_selection(node n, bool choice) {

	choices.push(std::make_pair(n, choice));
	formula->set_node(n, choice);
	DEBUG_print_assignment(*formula);

}





namespace {
	auto rand_engine = std::mt19937_64(std::random_device()());
}

boost::optional<node>
dpll_solver::choose_next_node(node_choice_mode mode) const {

	auto& assign = formula->get_incomplete_assignment();
	auto& assign_map = assign.data;

	// Find unset node
	const auto node_choice_pred = [](const auto& pair) {
		return boost::indeterminate(pair.second);
	};

	// If none, return no node
	if (std::none_of(assign_map.cbegin(), assign_map.cend(), node_choice_pred))
		return boost::optional<node>();

	// Otherwise, return node based on mode
	using iterator = incomplete_assignment::map::const_iterator;
	iterator next_node;
	switch (mode) {
	case node_choice_mode::Next:
		next_node = std::find_if(
			assign_map.cbegin(), assign_map.cend(), node_choice_pred);
	case node_choice_mode::Last:
		next_node = std::find_if(
			assign_map.crbegin(), assign_map.crend(), node_choice_pred).base();
	case node_choice_mode::Random:
		next_node = alphali::random_find_if(
			assign_map.cbegin(), assign_map.cend(), node_choice_pred, rand_engine);
	}

	auto node = assign.node_to_vertex_map->right.at(next_node->first);

	return node;
	
}
