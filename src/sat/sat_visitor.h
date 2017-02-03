#pragma once

#include "boost/graph/visitors.hpp"



namespace sat {

	template<class impl_visitor>
	struct sat_vert_visitor :
		public boost::base_visitor<sat_vert_visitor<impl_visitor>> {
	
		// Vertex visitor, so make sure this is an vertex filter.

		// Triggered when vertex is encountered
		template<class Vertex, class Graph>
		void operator()(Vertex v, Graph& g) {

			// Split depending on whether vert is node or clause
			auto& prop = g[v];
			switch(prop.kind) {
			case vert_prop::Node:
				static_cast<impl_visitor*>(this)->node_event(g, v, prop);
				break;
			case vert_prop::Clause:
				static_cast<impl_visitor*>(this)->clause_event(g, v, prop);
				break;
			}

		}

	};



	template<class impl_visitor>
	struct sat_edge_visitor :
		public boost::base_visitor<sat_edge_visitor<impl_visitor>> {
	
		// Edge visitor, so make sure this is an edge filter.

		// Triggered when edge is encountered
		template<class Edge, class Graph>
		void operator()(Edge e, Graph& g) {

			// Find which vert is node and which is clause
			auto vert_node = boost::source(e, g);
			auto vert_clause = boost::target(e, g);
			if (g[vert_node].kind == vert_prop::Clause)
				std::swap(vert_node, vert_clause);

			static_cast<impl_visitor*>(this)->edge_event(
				g, e, g[e], vert_node, vert_clause);
		
		}

	};

}
