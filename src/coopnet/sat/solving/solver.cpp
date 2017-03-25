#include "solver.h"
#include "coopnet/sat/problem/assignment.h"

using namespace coopnet;



std::ostream& coopnet::operator<<(
	std::ostream& os, SolutionStatus status) {

	switch (status) {
	case coopnet::SolutionStatus::Satisfied:
		os << "Satisfied";
		break;
	case coopnet::SolutionStatus::Partial:
		os << "Partial";
		break;
	case coopnet::SolutionStatus::Unsatisfied:
		os << "Unsatisfiable";
		break;
	case coopnet::SolutionStatus::Undetermined:
		os << "Undetermined";
		break;
	}

	return os;

}



std::ostream& coopnet::operator<<(
	std::ostream& os, const Solution& solution) {

	os << solution.status << std::endl;

	switch (solution.status) {
	case SolutionStatus::Satisfied:
		os << "Assignment:" << std::endl;
		os << *solution.assignment << std::endl;
		break;
	case SolutionStatus::Partial:
		os << "Assignment:" << std::endl;
		os << *solution.assignment << std::endl;
		os << "Num clauses failed: " << solution.num_failed << std::endl;
		break;
	case SolutionStatus::Unsatisfied:
	case SolutionStatus::Undetermined:
		break;
	}

	return os;

}



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
