#pragma once



namespace sat {

	enum class dpll_vert_status {
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

	inline std::ostream& operator<<(std::ostream& os, dpll_vert_status status) {
		switch (status) {
		case dpll_vert_status::Active:
			return os << "Active";
		case dpll_vert_status::Inactive:
			return os << "Inactive";
		case dpll_vert_status::SetToTrue:
			return os << "SetToTrue";
		case dpll_vert_status::SetToFalse:
			return os << "SetToFalse";
		case dpll_vert_status::Remove:
			return os << "Remove";
		default:
			return os;
		}
	}

	enum class dpll_edge_status {
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

	inline std::ostream& operator<<(std::ostream& os, dpll_edge_status status) {
		switch (status) {
		case dpll_edge_status::Active:
			return os << "Active";
		case dpll_edge_status::Inactive:
			return os << "Inactive";
		case dpll_edge_status::SatisfyClause:
			return os << "SatisfyClause";
		case dpll_edge_status::ConstrainNode:
			return os << "ConstrainNode";
		case dpll_edge_status::Remove:
			return os << "Remove";
		default:
			return os;
		}
	}

}
