#pragma once

#include <stack>
#include "sat/graph.h"
#include "boost/variant.hpp"
#include "dpll_status.h"



namespace sat {

	using incomplete_assignment_prune_data
		= std::pair<vertex_descriptor, boost::tribool>;
	using vertex_prune_data = std::pair<vertex_descriptor, dpll_vert_status>;
	using edge_prune_data = std::pair<edge_descriptor, dpll_edge_status>;

	struct prune_action {

		enum class prune_object { Assignment, Vertex, Edge };

		prune_object type;
		boost::variant<incomplete_assignment_prune_data,
			vertex_prune_data, edge_prune_data> supp_data;

		explicit prune_action(incomplete_assignment_prune_data prune_data) {
			type = prune_object::Assignment;
			supp_data = prune_data;
		}

		explicit prune_action(vertex_prune_data prune_data) {
			type = prune_object::Vertex;
			supp_data = prune_data;
		}

		explicit prune_action(edge_prune_data prune_data) {
			type = prune_object::Edge;
			supp_data = prune_data;
		}

	};

	struct prune_stack {

		std::stack<prune_action> data;

	};

}
