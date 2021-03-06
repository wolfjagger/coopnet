#pragma once

#include <algorithm>
#include "coopnet/graph/mutable/reversable_graph.h"
#include "coopnet/sat/visitor/sat_visitor.h"



namespace coopnet {

	template<class ImplVisitor>
	struct PrunedSatVertVisitor :
		public boost::base_visitor<PrunedSatVertVisitor<ImplVisitor>> {

		// Vertex visitor, so make sure this is an vertex filter.

	private:

		using Me = PrunedSatVertVisitor<ImplVisitor>;

	public:

		// Triggered when vertex is encountered
		template<typename SatGraph>
		void operator()(VertDescriptor v, const SatGraph& g) {

			auto& prop = g[v];
			switch (prop.kind()) {
				// Split depending on whether vert is node or clause
			case VertKind::Node:
				if(prop.node().pruneStatus == PruneStatus::Active) {
					static_cast<ImplVisitor*>(this)->node_event(g, v, prop.node());
				}
				break;
			case VertKind::Clause:
				if (prop.clause().pruneStatus == PruneStatus::Active) {
					static_cast<ImplVisitor*>(this)->clause_event(g, v, prop.clause());
				}
				break;
			}

		}



	protected:

		template<class SatGraph>
		static bool any_active_edge(VertDescriptor v, const SatGraph& g) {
			auto edgePair = boost::out_edges(v, g);
			return std::any_of(edgePair.first, edgePair.second, is_active_pred(g));
		}

		template<class SatGraph>
		static size_t count_active_edges(VertDescriptor v, const SatGraph& g) {
			auto edgePair = boost::out_edges(v, g);
			return std::count_if(edgePair.first, edgePair.second, is_active_pred(g));
		}

		template<class SatGraph>
		static auto find_active_edge(VertDescriptor v, const SatGraph& g) {
			auto edgePair = boost::out_edges(v, g);
			return std::find_if(edgePair.first, edgePair.second, is_active_pred(g));
		}

		template<class SatGraph, typename Pred>
		static auto find_if_active_edge(VertDescriptor v, const SatGraph& g, Pred p) {
			auto edgePair = boost::out_edges(v, g);
			return std::find_if(edgePair.first, edgePair.second, [&p, &g](EdgeDescriptor e) {
				return is_active_edge(e, g) && p(e);
			});
		}

		template<class SatGraph, typename UnaryFcn>
		static void for_each_active_edge(VertDescriptor v, const SatGraph& g, UnaryFcn p) {
			auto edgePair = boost::out_edges(v, g);
			std::for_each(edgePair.first, edgePair.second, [&p, &g](EdgeDescriptor e) {
				if (is_active_edge(e, g)) p(e);
			});
		}

		template<class SatGraph, typename Pred>
		static bool all_of_active_edges(VertDescriptor v, const SatGraph& g, Pred p) {
			auto edgePair = boost::out_edges(v, g);
			return std::all_of(edgePair.first, edgePair.second, [&p, &g](EdgeDescriptor e) {
				return is_active_edge(e, g) && p(e);
			});
		}

		template<class SatGraph>
		static bool is_active_edge(EdgeDescriptor e, const SatGraph& g) {
			return g[e].pruneStatus == PruneStatus::Active;
		}

	private:

		template<class SatGraph>
		static auto is_active_pred(const SatGraph& g) {
			return [&g](EdgeDescriptor e) { return is_active_edge(e, g); };
		}

	};



	template<class ImplVisitor>
	struct PruneSatEdgeVisitor :
		public boost::base_visitor<PruneSatEdgeVisitor<ImplVisitor>> {

		// Edge visitor, so make sure this is an edge filter.

	private:

		using Me = PrunedSatVertVisitor<ImplVisitor>;

	public:

		// Triggered when edge is encountered
		template<typename SatGraph>
		void operator()(EdgeDescriptor e, const SatGraph& g) {

			auto& prop = g[e];
			if(prop.pruneStatus == PruneStatus::Active) {
				
				// Find which vert is node and which is clause
				auto vert_node = boost::source(e, g);
				auto vert_clause = boost::target(e, g);
				if (g[vert_node].kind() == VertKind::Clause)
					std::swap(vert_node, vert_clause);

				static_cast<ImplVisitor*>(this)->edge_event(
					g, e, prop, vert_node, vert_clause);

			}

		}

	};

}
