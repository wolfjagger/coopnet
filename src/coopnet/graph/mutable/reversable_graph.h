#pragma once

#include "coopnet/graph/base/graph_util.h"
#include "reverse_info.h"



namespace coopnet {

	struct ReversableSatProp {

		struct Node : public PruneSatProp::Node, public AssignSatProp::Node {};
		struct Clause : public PruneSatProp::Clause, public AssignSatProp::Clause {};
		struct Edge : public PruneSatProp::Edge, public AssignSatProp::Edge {};

	};


	template<typename SatProp>
	class ReversableSatGraph {

	private:

		constexpr static bool DEBUG = false;

		TranslatedSatGraph<SatProp> graph;
		ReverseStack reverseStack;

		// Connected components members
		size_t numConnectedComponents;
		std::vector<VertDescriptor> connectedComponentEntryPts;

	public:

		ReversableSatGraph() = default;

		ReversableSatGraph(
			const BaseSatGraph& original, const SatGraphTranslator& origTranslator);



		SatGraph<SatProp>& get_graph() { return graph.graph; }
		const SatGraph<SatProp>& get_graph() const { return graph.graph; }

		const SatGraphTranslator& get_translator() const { return graph.translator; }

		ReverseStack& reverse_stack() { return reverseStack; }
		const ReverseStack& reverse_stack() const { return reverseStack; }



		PruneStatus get_vert_status(VertDescriptor v) const;
		void set_vert_status(VertDescriptor v, PruneStatus newStatus);

		PruneStatus get_edge_status(EdgeDescriptor e) const;
		void set_edge_status(EdgeDescriptor e, PruneStatus newStatus);

		boost::tribool get_assignment(VertDescriptor v) const;
		void set_assignment(VertDescriptor v, boost::tribool newValue);
		bool is_indeterminate_node(VertDescriptor v) const;
		bool is_indeterminate() const;



		void reverse_to_vert(VertDescriptor v);
		void reset_all();



		size_t num_connected_components() const;
		const std::vector<VertDescriptor>&
			connected_component_entry_pts() const;



		template<typename PruneVisitor>
		void visit(PruneVisitor& v);

		template<typename PruneVisitor>
		void visit(PruneVisitor& v) const;

	};

	using BaseRevSatGraph = ReversableSatGraph<ReversableSatProp>;

#include "reversable_graph.inl"

}
