#include "dpll_solver.h"
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
	 *   if DPLL-recursive(F|-x, p || {-x}) = SAT then return SAT
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
	
	partial_graph = prob.get_graph();
	partial_assign = assignment();

	return partial_solve();

}



auto dpll_solver::partial_solve() -> solve_return {

	// Use property map (color, bool, whatever is needed) to update necessary
	//  information in an agreeable way so that partial_graph can be traversed
	//  in a way that ignores already set nodes/clauses, knows if T has been set
	//  for a node but not F, knows which literal to backtrack to, etc.
	// This single solver might already have enough choices to make subclasses or
	//  more likely composition to explore the interesting ones.
	// Obviously, we will be using visitors to perform the interesting actions.
	// Make a visitor for each detachable subtask, e.g. unit clauses and pure literals.
	// Could try to modify the traversal color map so that it doesn't go into
	//  nodes/clauses that are already set in the partial assignment/reduced problem!

	remove_unit_clauses();
	remove_pure_literals();

	return solve_return(solution_status::Undetermined, std::shared_ptr<assignment>());

}


void dpll_solver::remove_unit_clauses() {
	


}

void dpll_solver::remove_pure_literals() {
	


}
