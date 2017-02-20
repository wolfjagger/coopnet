#pragma once



namespace sat {

	enum class DPLLVertStatus {
		// Indicates vert is still relevant and not in transition
		Active,
		// Vert is irrelevant
		Inactive,
		// Node should be set to true when visited
		SetToTrue,
		// Node should be set to true when visited
		SetToFalse,
		// Clause should be removed when visited
		Remove
	};

	inline std::ostream& operator<<(std::ostream& os, DPLLVertStatus status) {
		switch (status) {
		case DPLLVertStatus::Active:
			return os << "Active";
		case DPLLVertStatus::Inactive:
			return os << "Inactive";
		case DPLLVertStatus::SetToTrue:
			return os << "SetToTrue";
		case DPLLVertStatus::SetToFalse:
			return os << "SetToFalse";
		case DPLLVertStatus::Remove:
			return os << "Remove";
		default:
			return os;
		}
	}

	enum class DPLLEdgeStatus {
		// Indicates edge is still relevant, but shouldn't be traversed
		Active,
		// Edge is irrelevant
		Inactive,
		// Node has been set and satisfies clause
		SatisfyClause,
		// Clause can only be satisfied by constraining this edge's node
		ConstrainNode,
		// Node has been set != edge sgn or clause has been satisfied but not by this edge's node
		Remove
	};

	inline std::ostream& operator<<(std::ostream& os, DPLLEdgeStatus status) {
		switch (status) {
		case DPLLEdgeStatus::Active:
			return os << "Active";
		case DPLLEdgeStatus::Inactive:
			return os << "Inactive";
		case DPLLEdgeStatus::SatisfyClause:
			return os << "SatisfyClause";
		case DPLLEdgeStatus::ConstrainNode:
			return os << "ConstrainNode";
		case DPLLEdgeStatus::Remove:
			return os << "Remove";
		default:
			return os;
		}
	}

}
