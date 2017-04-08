#pragma once

#include "boost/graph/visitors.hpp"
#include "coopnet/graph/base/graph.h"



namespace coopnet {

	template<class ImplVisitor>
	struct SatVertVisitor :
		public boost::base_visitor<SatVertVisitor<ImplVisitor>> {
	
		// Vertex visitor, so make sure this is an vertex filter.

		// Triggered when vertex is encountered
		template<typename SatGraph>
		void operator()(VertDescriptor v, const SatGraph& g) {

			// Split depending on whether vert is node or clause
			auto& prop = g[v];
			switch(prop.kind) {
			case VertKind::Node:
				static_cast<ImplVisitor*>(this)->node_event(g, v, prop.node());
				break;
			case VertKind::Clause:
				static_cast<ImplVisitor*>(this)->clause_event(g, v, prop.clause());
				break;
			}

		}

	};



	template<class ImplVisitor>
	struct SatEdgeVisitor :
		public boost::base_visitor<SatEdgeVisitor<ImplVisitor>> {
	
		// Edge visitor, so make sure this is an edge filter.

		// Triggered when edge is encountered
		template<typename SatGraph>
		void operator()(EdgeDescriptor e, const SatGraph& g) {

			// Find which vert is node and which is clause
			auto vert_node = boost::source(e, g);
			auto vert_clause = boost::target(e, g);
			if (g[vert_node].kind() == VertKind::Clause)
				std::swap(vert_node, vert_clause);

			static_cast<ImplVisitor*>(this)->edge_event(
				g, e, g[e], vert_node, vert_clause);
		
		}

	};

}
