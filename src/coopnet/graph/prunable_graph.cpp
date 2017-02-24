#include "prunable_graph.h"
#include "graph_util.h"

using namespace coopnet;



PrunableSatGraph::PrunableSatGraph(const BaseSatGraph& original) :
	base(original),
	pruneInfo(base.get()) {

	connectedComponentEntryPts = graph_util::calculate_connected_components(original);
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
