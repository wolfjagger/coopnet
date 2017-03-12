#pragma once

#include "coopnet/graph/graph_util.h"
#include "prune_info.h"



namespace coopnet {

	template<typename VProp, typename EProp>
	class ExtendedSatGraph {

	private:

		constexpr static bool DEBUG = false;

		SatGraph<VProp, EProp> graph;
		PruneStack pruneStack;

		// Connected components members
		size_t numConnectedComponents;
		std::vector<VertDescriptor> connectedComponentEntryPts;

	public:

		ExtendedSatGraph() = default;

		ExtendedSatGraph(const BaseSatGraph& original);



		SatGraph<VProp, EProp>& get_graph() { return graph; }
		const SatGraph<VProp, EProp>& get_graph() const { return graph; }

		PruneStack& prune_stack() { return pruneStack; }
		const PruneStack& prune_stack() const { return pruneStack; }



		PruneStatus ExtendedSatGraph::get_vert_status(VertDescriptor v) const;
		void ExtendedSatGraph::set_vert_status(VertDescriptor v, PruneStatus newStatus);

		PruneStatus ExtendedSatGraph::get_edge_status(EdgeDescriptor e) const;
		void ExtendedSatGraph::set_edge_status(EdgeDescriptor e, PruneStatus newStatus);

		boost::tribool ExtendedSatGraph::get_assignment(VertDescriptor v) const;
		void ExtendedSatGraph::set_assignment(VertDescriptor v, boost::tribool newValue);
		bool ExtendedSatGraph::is_indeterminate_node(VertDescriptor v) const;
		bool ExtendedSatGraph::is_indeterminate() const;



		void reverse_to_vert(VertDescriptor v);
		void reset_prune();



		size_t num_connected_components() const;
		const std::vector<VertDescriptor>&
			connected_component_entry_pts() const;



		template<typename PruneVisitor>
		void visit(PruneVisitor& v);

		template<typename PruneVisitor>
		void visit(PruneVisitor& v) const;

	private:

		static SatGraph<VProp, EProp> create_extended_from_base(const BaseSatGraph& original);
		
	};

#include "extended_graph.inl"

}
