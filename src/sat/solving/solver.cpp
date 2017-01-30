#include "solver.h"

using namespace sat;



auto complete_solver::solve(const problem& prob) -> solve_return {
	
	auto solution = do_solve(prob);

	if (solution.first == solution_status::Undetermined)
		throw std::exception("Complete solver has undetermined solution.");

	return solution;

}



auto incomplete_solver::solve(const problem& prob) -> solve_return {
			
	for (unsigned int i = 0; i < retry_count(); ++i) {
		auto possible_solution = try_single_solve(prob);
		if (possible_solution.first != solution_status::Undetermined) {
			return possible_solution;
		}
	}

	return std::make_pair(solution_status::Undetermined, nullptr);

}
