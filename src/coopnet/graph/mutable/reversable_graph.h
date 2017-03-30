#pragma once

#include "coopnet/graph/graph_util.h"
#include "reverse_info.h"



namespace coopnet {

	struct ReversableSatVProp : public PruneSatVProp, public AssignSatVProp { };
	struct ReversableSatEProp : public PruneSatEProp, public AssignSatEProp { };


	template<typename VProp, typename EProp>
	class ReversableSatGraph {

	private:

		constexpr static bool DEBUG = false;

		SatGraph<VProp, EProp> graph;
		ReverseStack reverseStack;

		// Connected components members
		size_t numConnectedComponents;
		std::vector<VertDescriptor> connectedComponentEntryPts;

	public:

		ReversableSatGraph() = default;

		ReversableSatGraph(const BaseSatGraph& original);



		SatGraph<VProp, EProp>& get_graph() { return graph; }
		const SatGraph<VProp, EProp>& get_graph() const { return graph; }

		ReverseStack& reverse_stack() { return reverseStack; }
		const ReverseStack& reverse_stack() const { return reverseStack; }



		PruneStatus ReversableSatGraph::get_vert_status(VertDescriptor v) const;
		void ReversableSatGraph::set_vert_status(VertDescriptor v, PruneStatus newStatus);

		PruneStatus ReversableSatGraph::get_edge_status(EdgeDescriptor e) const;
		void ReversableSatGraph::set_edge_status(EdgeDescriptor e, PruneStatus newStatus);

		boost::tribool ReversableSatGraph::get_assignment(VertDescriptor v) const;
		void ReversableSatGraph::set_assignment(VertDescriptor v, boost::tribool newValue);
		bool ReversableSatGraph::is_indeterminate_node(VertDescriptor v) const;
		bool ReversableSatGraph::is_indeterminate() const;



		void reverse_to_vert(VertDescriptor v);
		void reset_all();



		size_t num_connected_components() const;
		const std::vector<VertDescriptor>&
			connected_component_entry_pts() const;



		template<typename PruneVisitor>
		void visit(PruneVisitor& v);

		template<typename PruneVisitor>
		void visit(PruneVisitor& v) const;

	private:

		static SatGraph<VProp, EProp> create_reversable_from_base(const BaseSatGraph& original);
		
	};

	using BaseRevSatGraph = ReversableSatGraph<ReversableSatVProp, ReversableSatEProp>;

#include "reversable_graph.inl"

}
