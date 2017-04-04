#pragma once

#include <ostream>



namespace coopnet {

	enum class WalkVertStatus {
		// Nothing
		Default,
		// Clause is currently satisfied
		Satisfied,
		// Clause is currently unsatisfied
		Unsatisfied,
	};

	inline std::ostream& operator<<(std::ostream& os, WalkVertStatus status) {
		switch (status) {
		case WalkVertStatus::Default:
			return os << "Default";
		case WalkVertStatus::Satisfied:
			return os << "Satisfied";
		case WalkVertStatus::Unsatisfied:
			return os << "Unsatisfied";
		default:
			return os;
		}
	}

}
