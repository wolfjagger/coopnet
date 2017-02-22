#pragma once



namespace sat {

	enum class DPLLVertStatus {
		// Nothing
		Default,
		// Node should be set to true when visited
		SetToTrue,
		// Node should be set to true when visited
		SetToFalse,
		// Clause should be removed when visited
		Remove
	};

	inline std::ostream& operator<<(std::ostream& os, DPLLVertStatus status) {
		switch (status) {
		case DPLLVertStatus::Default:
			os << "Default";
		case DPLLVertStatus::SetToTrue:
			os << "SetToTrue";
		case DPLLVertStatus::SetToFalse:
			os << "SetToFalse";
		case DPLLVertStatus::Remove:
			os << "Remove";
		}
		return os;
	}

	enum class DPLLEdgeStatus {
		//Nothing
		Default,
		// Node has been set and satisfies clause
		SatisfyClause,
		// Clause can only be satisfied by constraining this edge's node
		ConstrainNode,
		// Node has been set != edge sgn or clause has been satisfied but not by this edge's node
		Remove
	};

	inline std::ostream& operator<<(std::ostream& os, DPLLEdgeStatus status) {
		switch (status) {
		case DPLLEdgeStatus::Default:
			os << "Default";
		case DPLLEdgeStatus::SatisfyClause:
			os << "SatisfyClause";
		case DPLLEdgeStatus::ConstrainNode:
			os << "ConstrainNode";
		case DPLLEdgeStatus::Remove:
			os << "Remove";
		}
		return os;
	}

}
