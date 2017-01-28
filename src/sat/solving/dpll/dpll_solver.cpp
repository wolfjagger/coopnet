#include "dpll_solver.h"
#include "alphali/Containers/random_iterator.h"
#include "boost/logic/tribool.hpp"
#include "sat/problem.h"

using namespace sat;



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
	
	formula = dpll_formula(prob);

	return partial_solve();

}



auto dpll_solver::partial_solve() -> solve_return {

	return solve_return(solution_status::Undetermined, std::shared_ptr<assignment>());

}



namespace {
	auto rand_engine = std::mt19937_64(std::random_device()());
}

vertex_descriptor dpll_solver::choose_next_node(node_choice_mode mode) const {

	auto& assign_map = formula.get_incomplete_assignment().data;

	const auto node_choice_pred = [](const auto& pair) {
		return pair.second == boost::indeterminate;
	};

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

	return next_node->first;
	
}
