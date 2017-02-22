#include "prunable_graph.h"

using namespace sat;



PrunableSatGraph::PrunableSatGraph(const SatGraph& original) :
	base(original),
	pruneInfo(base.get()) {

	connectedComponentEntryPts = calculate_connected_components(original);
	numConnectedComponents = connectedComponentEntryPts.size();

}



void PrunableSatGraph::reset_prune() {

	pruneInfo.reset_prune();

}



size_t PrunableSatGraph::num_connected_components() const {
	return numConnectedComponents;
}
const std::vector<VertDescriptor>&
PrunableSatGraph::connected_component_entry_pts() const {
	return connectedComponentEntryPts;
}
