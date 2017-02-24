#pragma once

#include "prune_info.h"



namespace sat {

	class PrunableSatGraph {

	private:

		std::reference_wrapper<const BaseSatGraph> base;
		PruneInfo pruneInfo;

		// Connected components members
		size_t numConnectedComponents;
		std::vector<VertDescriptor> connectedComponentEntryPts;

	public:

		PrunableSatGraph() = default;

		PrunableSatGraph(const BaseSatGraph& original);



		PruneInfo& prune_info() {
			return pruneInfo;
		}
		const PruneInfo& prune_info() const {
			return pruneInfo;
		}

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
