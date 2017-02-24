#pragma once

#include <algorithm>
#include "coopnet/graph/prunable_graph.h"
#include "sat_visitor.h"



namespace sat {

	template<class ImplVisitor, typename TypePruneInfo,
		typename = std::enable_if_t<
		std::is_same_v<PruneInfo, std::remove_cv_t<TypePruneInfo>>>>
	struct PruneSatVertVisitor :
		public boost::base_visitor<PruneSatVertVisitor<ImplVisitor, TypePruneInfo>> {

		// Vertex visitor, so make sure this is an vertex filter.

	private:

		using Me = PruneSatVertVisitor<ImplVisitor, TypePruneInfo>;

		// Needs to be set to match the PrunableSatGraph that will be visited
		std::reference_wrapper<TypePruneInfo> pruneInfo;

	public:

		PruneSatVertVisitor(TypePruneInfo& initPruneInfo) :
			pruneInfo(initPruneInfo) {}

		// Triggered when vertex is encountered
		void operator()(VertDescriptor v, const BaseSatGraph& g) {

			if(prune_info().get_vert_status(v) == PruneStatus::Active) {

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



		void set_prune_info(TypePruneInfo& newInfo) {
			pruneInfo = newInfo;
		}



	protected:

		TypePruneInfo& prune_info() {
			return pruneInfo;
		}
		const TypePruneInfo& prune_info() const {
			return pruneInfo;
		}



		bool any_active_edge(VertDescriptor v, const BaseSatGraph& g) const {
			auto edgePair = boost::out_edges(v, g);
			return std::any_of(edgePair.first, edgePair.second, Me::active_predicate(*this));
		}

		size_t count_active_edges(VertDescriptor v, const BaseSatGraph& g) const {
			auto edgePair = boost::out_edges(v, g);
			return std::count_if(edgePair.first, edgePair.second, Me::active_predicate(*this));
		}

		auto find_active_edge(VertDescriptor v, const BaseSatGraph& g) const {
			auto edgePair = boost::out_edges(v, g);
			return std::find_if(edgePair.first, edgePair.second, Me::active_predicate(*this));
		}

		template<typename Pred>
		auto find_if_active_edge(VertDescriptor v, const BaseSatGraph& g, Pred p) const {
			auto edgePair = boost::out_edges(v, g);
			return std::find_if(edgePair.first, edgePair.second, [this, &p](EdgeDescriptor e) {
				return is_active_edge(e) && p(e);
			});
		}

		template<typename UnaryFcn>
		void for_each_active_edge(VertDescriptor v, const BaseSatGraph& g, UnaryFcn p) const {
			auto edgePair = boost::out_edges(v, g);
			std::for_each(edgePair.first, edgePair.second, [this, &p](EdgeDescriptor e) {
				if (is_active_edge(e)) p(e);
			});
		}

		template<typename Pred>
		bool all_of_active_edges(VertDescriptor v, const BaseSatGraph& g, Pred p) const {
			auto edgePair = boost::out_edges(v, g);
			return std::all_of(edgePair.first, edgePair.second, [this, &p](EdgeDescriptor e) {
				return is_active_edge(e) && p(e);
			});
		}

		bool is_active_edge(EdgeDescriptor e) const {
			return prune_info().get_edge_status(e) == PruneStatus::Active;
		}

	private:

		static auto active_predicate(const Me& me) {
			return [&me](EdgeDescriptor e) {
				return me.is_active_edge(e);
			};
		}

	};



	template<class ImplVisitor, typename TypePruneInfo,
		typename = std::enable_if_t<
		std::is_same_v<PruneInfo, std::remove_cv_t<TypePruneInfo>>>>
	struct PruneSatEdgeVisitor :
		public boost::base_visitor<PruneSatEdgeVisitor<ImplVisitor, TypePruneInfo>> {

		// Edge visitor, so make sure this is an edge filter.

	private:

		using Me = PruneSatVertVisitor<ImplVisitor, TypePruneInfo>;

		// Needs to be set to match the PrunableSatGraph that will be visited
		TypePruneInfo& pruneInfo;

	public:

		PruneSatEdgeVisitor(TypePruneInfo& initPruneInfo) :
			pruneInfo(initPruneInfo) {}

		// Triggered when edge is encountered
		void operator()(EdgeDescriptor e, const BaseSatGraph& g) {

			if(prune_info().get_edge_status(e) == PruneStatus::Active) {
				
				// Find which vert is node and which is clause
				auto vert_node = boost::source(e, g);
				auto vert_clause = boost::target(e, g);
				if (g[vert_node].kind == VertProp::Clause)
					std::swap(vert_node, vert_clause);

				static_cast<ImplVisitor*>(this)->edge_event(
					g, e, g[e], vert_node, vert_clause);

			}

		}



		void set_prune_info(TypePruneInfo& newInfo) {
			pruneInfo = newInfo;
		}



	protected:

		TypePruneInfo& prune_info() {
			return pruneInfo;
		}

	};

}
