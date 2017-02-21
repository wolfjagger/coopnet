#pragma once

#include "coopnet/graph/prunable_graph.h"
#include "sat_visitor.h"



namespace sat {

	template<class ImplVisitor>
	struct PruneSatVertVisitor :
		public boost::base_visitor<PruneSatVertVisitor<ImplVisitor>> {

		// Vertex visitor, so make sure this is an vertex filter.

		// Needs to be set to match the PrunableSatGraph that will be visited
		PrunablePropMaps activityMaps;

		// Triggered when vertex is encountered
		void operator()(VertDescriptor v, const SatGraph& g) {

			if(activityMaps.vertStatus[v]) {

				// Split depending on whether vert is node or clause
				auto& prop = g[v];
				switch (prop.kind) {
				case VertProp::Node:
					static_cast<ImplVisitor*>(this)->node_event(g, v, prop);
					break;
				case VertProp::Clause:
					static_cast<ImplVisitor*>(this)->clause_event(g, v, prop);
					break;
				}

			}

		}

	};



	template<class ImplVisitor>
	struct PruneSatEdgeVisitor :
		public boost::base_visitor<PruneSatEdgeVisitor<ImplVisitor>> {

		// Edge visitor, so make sure this is an edge filter.

		// Needs to be set to match the PrunableSatGraph that will be visited
		PrunablePropMaps activityMaps;

		// Triggered when edge is encountered
		void operator()(EdgeDescriptor e, const SatGraph& g) {

			if(activityMaps.edgeStatus[e]) {
				
				// Find which vert is node and which is clause
				auto vert_node = boost::source(e, g);
				auto vert_clause = boost::target(e, g);
				if (g[vert_node].kind == VertProp::Clause)
					std::swap(vert_node, vert_clause);

				static_cast<ImplVisitor*>(this)->edge_event(
					g, e, g[e], vert_node, vert_clause);

			}

		}

	};

}
