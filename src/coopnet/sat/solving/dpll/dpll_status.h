#pragma once

#include <ostream>



namespace coopnet {

	enum class DPLLNodeStatus {
		// Nothing
		Default,
		// Node should be set to true when visited
		SetToTrue,
		// Node should be set to true when visited
		SetToFalse,
	};

	inline std::ostream& operator<<(std::ostream& os, DPLLNodeStatus status) {
		switch (status) {
		case DPLLNodeStatus::Default:
			return os << "Default";
		case DPLLNodeStatus::SetToTrue:
			return os << "SetToTrue";
		case DPLLNodeStatus::SetToFalse:
			return os << "SetToFalse";
		default:
			return os;
		}
	}

	enum class DPLLClauseStatus {
		// Nothing
		Default,
		// Clause should be removed when visited
		Remove
	};

	inline std::ostream& operator<<(std::ostream& os, DPLLClauseStatus status) {
		switch (status) {
		case DPLLClauseStatus::Default:
			return os << "Default";
		case DPLLClauseStatus::Remove:
			return os << "Remove";
		default:
			return os;
		}
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
			return os << "Default";
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
