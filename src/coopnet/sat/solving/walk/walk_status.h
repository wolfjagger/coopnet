#pragma once

#include <ostream>



namespace coopnet {

	enum class WalkVertStatus {
		// Nothing
		Default,
		// Node should be flipped when visited
		Flip,
		// Clause is currently satisfied
		Satisfied,
		// Clause is currently unsatisfied
		Unsatisfied,
	};

	inline std::ostream& operator<<(std::ostream& os, WalkVertStatus status) {
		switch (status) {
		case WalkVertStatus::Default:
			return os << "Default";
		case WalkVertStatus::Flip:
			return os << "Flip";
		case WalkVertStatus::Satisfied:
			return os << "Satisfied";
		case WalkVertStatus::Unsatisfied:
			return os << "Unsatisfied";
		default:
			return os;
		}
	}

	enum class WalkEdgeStatus {
		//Nothing
		Default,
		// Node has been set and satisfies clause
		SatisfyClause,
		// Node has been set and does not satisfy clause
		ReevaluateClause,
	};

	inline std::ostream& operator<<(std::ostream& os, WalkEdgeStatus status) {
		switch (status) {
		case WalkEdgeStatus::Default:
			return os << "Default";
		case WalkEdgeStatus::SatisfyClause:
			return os << "SatisfyClause";
		case WalkEdgeStatus::ReevaluateClause:
			return os << "ReevaluateClause";
		default:
			return os;
		}
	}

}
