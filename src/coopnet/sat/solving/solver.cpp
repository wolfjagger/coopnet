#include "solver.h"
#include "node_chooser.h"

using namespace sat;



Solver::Solver(std::unique_ptr<NodeChooser> chooser) :
	n_chooser(std::move(chooser)) {

}

Solver::~Solver() {

}



CompleteSolver::CompleteSolver(std::unique_ptr<NodeChooser> chooser) :
	Solver(std::move(chooser)) {

}

CompleteSolver::~CompleteSolver() {

}


auto CompleteSolver::solve(const Problem& prob) -> SolveReturn {
	
	auto solution = do_solve(prob);

	if (solution.first == SolutionStatus::Undetermined)
		throw std::exception("Complete solver has undetermined solution.");

	return solution;

}



IncompleteSolver::IncompleteSolver(std::unique_ptr<NodeChooser> chooser) :
	Solver(std::move(chooser)) {

}

IncompleteSolver::~IncompleteSolver() {

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
