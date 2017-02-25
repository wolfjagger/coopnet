#pragma once

#include "prune_info.h"



namespace coopnet {

	class PrunableSatGraph {

	private:

		MutableSatGraph graph;
		PruneStack pruneStack;

		// Connected components members
		size_t numConnectedComponents;
		std::vector<VertDescriptor> connectedComponentEntryPts;

	public:

		PrunableSatGraph() = default;

		PrunableSatGraph(const BaseSatGraph& original);



		MutableSatGraph& get_graph() { return graph; }
		const MutableSatGraph& get_graph() const { return graph; }



		PruneStatus PrunableSatGraph::get_vert_status(VertDescriptor v) const;
		void PrunableSatGraph::set_vert_status(VertDescriptor v, PruneStatus newStatus);

		PruneStatus PrunableSatGraph::get_edge_status(EdgeDescriptor e) const;
		void PrunableSatGraph::set_edge_status(EdgeDescriptor e, PruneStatus newStatus);

		boost::tribool PrunableSatGraph::get_assignment(VertDescriptor v) const;
		void PrunableSatGraph::set_assignment(VertDescriptor v, boost::tribool newValue);
		bool PrunableSatGraph::is_indeterminate_node(VertDescriptor v) const;
		bool PrunableSatGraph::is_indeterminate() const;



		void reverse_to_vert(VertDescriptor v);
		void reset_prune();



		size_t num_connected_components() const;
		const std::vector<VertDescriptor>&
			connected_component_entry_pts() const;



		template<typename PruneVisitor>
		void visit(PruneVisitor& v) {

			v.set_prune_info(pruneInfo);

			visit_sat_graph(v, base,
				connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

		}

		template<typename PruneVisitor>
		void visit(PruneVisitor& v) const {

			v.set_prune_info(pruneInfo);

			visit_sat_graph(v, base,
				connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

		}
		
	};

}
