#pragma once

#include <ostream>
#include "coopnet/sat/problem/assignment.h"



namespace coopnet {

	struct Assignment;

	enum class SolutionStatus {
		Satisfied, Partial, Unsatisfied, Undetermined
	};

	inline std::ostream& operator<<(
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



	struct Solution {
		SolutionStatus status;
		std::shared_ptr<Assignment> assignment;
		unsigned int num_failed;
	};

	inline std::ostream& operator<<(
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

}
