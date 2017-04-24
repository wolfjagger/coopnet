#include "solver.h"

using namespace coopnet;



Solver::~Solver() { }



auto CompleteSolver::solve() -> Solution {
	
	auto solution = do_solve();

	if (solution.status == SolutionStatus::Undetermined)
		throw std::exception("Complete solver has undetermined solution.");

	return solution;

}



auto IncompleteSolver::solve() -> Solution {
			
	for (unsigned int i = 0; i < retry_count(); ++i) {
		auto possible_solution = try_single_solve();
		if (possible_solution.status != SolutionStatus::Undetermined) {
			return possible_solution;
		}
	}

	return Solution{ SolutionStatus::Undetermined, nullptr, 0 };

}
