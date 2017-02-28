#pragma once

#include <algorithm>
#include "coopnet/graph/mutable_graph.h"
#include "sat_visitor.h"



namespace coopnet {

	template<class ImplVisitor>
	struct PruneSatVertVisitor :
		public boost::base_visitor<PruneSatVertVisitor<ImplVisitor>> {

		// Vertex visitor, so make sure this is an vertex filter.

	private:

		using Me = PruneSatVertVisitor<ImplVisitor>;

	public:

		// Triggered when vertex is encountered
		template<typename SatGraph>
		void operator()(VertDescriptor v, const SatGraph& g) {

			auto& prop = g[v];
			if(prop.mutate.status == PruneStatus::Active) {

				// Split depending on whether vert is node or clause
				switch (prop.base.kind) {
				case BaseSatVProp::Node:
					static_cast<ImplVisitor*>(this)->node_event(g, v, prop);
					break;
				case BaseSatVProp::Clause:
					static_cast<ImplVisitor*>(this)->clause_event(g, v, prop);
					break;
				}

			}

		}



	protected:

		static bool any_active_edge(VertDescriptor v, const MutableSatGraph& g) {
			auto edgePair = boost::out_edges(v, g);
			return std::any_of(edgePair.first, edgePair.second, is_active_pred(g));
		}

		static size_t count_active_edges(VertDescriptor v, const MutableSatGraph& g) {
			auto edgePair = boost::out_edges(v, g);
			return std::count_if(edgePair.first, edgePair.second, is_active_pred(g));
		}

		static auto find_active_edge(VertDescriptor v, const MutableSatGraph& g) {
			auto edgePair = boost::out_edges(v, g);
			return std::find_if(edgePair.first, edgePair.second, is_active_pred(g));
		}

		template<typename Pred>
		static auto find_if_active_edge(VertDescriptor v, const MutableSatGraph& g, Pred p) {
			auto edgePair = boost::out_edges(v, g);
			return std::find_if(edgePair.first, edgePair.second, [&p, &g](EdgeDescriptor e) {
				return is_active_edge(e, g) && p(e);
			});
		}

		template<typename UnaryFcn>
		static void for_each_active_edge(VertDescriptor v, const MutableSatGraph& g, UnaryFcn p) {
			auto edgePair = boost::out_edges(v, g);
			std::for_each(edgePair.first, edgePair.second, [&p, &g](EdgeDescriptor e) {
				if (is_active_edge(e, g)) p(e);
			});
		}

		template<typename Pred>
		static bool all_of_active_edges(VertDescriptor v, const MutableSatGraph& g, Pred p) {
			auto edgePair = boost::out_edges(v, g);
			return std::all_of(edgePair.first, edgePair.second, [&p, &g](EdgeDescriptor e) {
				return is_active_edge(e, g) && p(e);
			});
		}

		static bool is_active_edge(EdgeDescriptor e, const MutableSatGraph& g) {
			return g[e].mutate.status == PruneStatus::Active;
		}

	private:

		static auto is_active_pred(const MutableSatGraph& g) {
			return [&g](EdgeDescriptor e) { return is_active_edge(e, g); };
		}

	};



	template<class ImplVisitor>
	struct PruneSatEdgeVisitor :
		public boost::base_visitor<PruneSatEdgeVisitor<ImplVisitor>> {

		// Edge visitor, so make sure this is an edge filter.

	private:

		using Me = PruneSatVertVisitor<ImplVisitor>;

	public:

		// Triggered when edge is encountered
		template<typename SatGraph>
		void operator()(EdgeDescriptor e, const SatGraph& g) {

			auto& prop = g[e];
			if(prop.mutate.status == PruneStatus::Active) {
				
				// Find which vert is node and which is clause
				auto vert_node = boost::source(e, g);
				auto vert_clause = boost::target(e, g);
				if (g[vert_node].base.kind == BaseSatVProp::Clause)
					std::swap(vert_node, vert_clause);

				static_cast<ImplVisitor*>(this)->edge_event(
					g, e, prop, vert_node, vert_clause);

			}

		}

	};

}
