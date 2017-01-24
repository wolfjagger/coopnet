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
