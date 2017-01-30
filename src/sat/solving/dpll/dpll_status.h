#pragma once



namespace sat {

	enum class dpll_vert_status {
		Active, Inactive, SetToTrue, SetToFalse, Remove
	};
	enum class dpll_edge_status {
		Active, Inactive, PushNodeToClause, PushClauseToNode
	};

}
