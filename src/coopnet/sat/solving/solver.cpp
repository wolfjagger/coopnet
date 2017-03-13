#include "solver.h"

using namespace coopnet;



auto CompleteSolver::solve(const Problem& prob) -> SolveReturn {
	
	auto solution = do_solve(prob);

	if (solution.first == SolutionStatus::Undetermined)
		throw std::exception("Complete solver has undetermined solution.");

	return solution;

}



auto IncompleteSolver::solve(const Problem& prob) -> SolveReturn {
			
	for (unsigned int i = 0; i < retry_count(); ++i) {
		auto possible_solution = try_single_solve(prob);
		if (possible_solution.first != SolutionStatus::Undetermined) {
			return possible_solution;
		}
	}

	return std::make_pair(SolutionStatus::Undetermined, nullptr);

}
