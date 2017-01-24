#pragma once

#include <stack>
#include "sat/graph.h"
#include "boost/variant.hpp"



namespace sat {

	struct prune_vertex_data {
		vertex_descriptor descriptor;
	};

	struct prune_edge_data {
		edge_descriptor descriptor;
		bool sgn;
	};

	struct prune_action {

		enum class prune_object { Vertex, Edge };

		prune_object type;
		boost::variant<prune_vertex_data, prune_edge_data> supp_data;

		explicit prune_action(prune_vertex_data prune_data) {
			type = prune_object::Vertex;
			supp_data = prune_data;
		}

		explicit prune_action(prune_edge_data prune_data) {
			type = prune_object::Edge;
			supp_data = prune_data;
		}

	};

	struct prune_stack {

		std::stack<prune_action> data;

	};

}
