#pragma once

#include <stack>
#include "boost/variant.hpp"
#include "coopnet/graph/graph.h"
#include "dpll_status.h"



namespace sat {

	using IncompleteAssignmentPruneData
		= std::pair<VertDescriptor, boost::tribool>;
	using VertPruneData = std::pair<VertDescriptor, DPLLVertStatus>;
	using EdgePruneData = std::pair<EdgeDescriptor, DPLLEdgeStatus>;

	struct PruneAction {

		enum class PruneObject { Assignment, Vertex, Edge };

		PruneObject type;
		boost::variant<IncompleteAssignmentPruneData,
			VertPruneData, EdgePruneData> supp_data;

		explicit PruneAction(IncompleteAssignmentPruneData prune_data) {
			type = PruneObject::Assignment;
			supp_data = prune_data;
		}

		explicit PruneAction(VertPruneData prune_data) {
			type = PruneObject::Vertex;
			supp_data = prune_data;
		}

		explicit PruneAction(EdgePruneData prune_data) {
			type = PruneObject::Edge;
			supp_data = prune_data;
		}

	};

	struct PruneStack {

		std::stack<PruneAction> data;

	};

}
