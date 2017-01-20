#include "tracer_solver.h"

using namespace sat;



auto tracer_solver::do_solve(const problem& prob) -> solve_return {

	return solve_return(solution_status::Undetermined, std::shared_ptr<assignment>());

}
