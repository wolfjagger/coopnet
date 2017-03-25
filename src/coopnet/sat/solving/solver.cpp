#include "solver.h"

using namespace coopnet;



auto CompleteSolver::solve(const Problem& prob) -> Solution {
	
	auto solution = do_solve(prob);

	if (solution.status == SolutionStatus::Undetermined)
		throw std::exception("Complete solver has undetermined solution.");

	return solution;

}



auto IncompleteSolver::solve(const Problem& prob) -> Solution {
			
	for (unsigned int i = 0; i < retry_count(); ++i) {
		auto possible_solution = try_single_solve(prob);
		if (possible_solution.status != SolutionStatus::Undetermined) {
			return possible_solution;
		}
	}

	return Solution{ SolutionStatus::Undetermined, nullptr, 0 };

}
