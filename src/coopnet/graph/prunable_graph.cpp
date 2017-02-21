#include "prunable_graph.h"

using namespace sat;



PrunableSatGraph::PrunableSatGraph() :
	base(),
	vertStatus(),
	edgeStatus() {

}

PrunableSatGraph::PrunableSatGraph(const SatGraph& original) :
	base(original),
	vertStatus(),
	edgeStatus() {

	emplace_all_active();

}

PrunableSatGraph::PrunableSatGraph(SatGraph&& original) :
	base(std::move(original)),
	vertStatus(),
	edgeStatus() {

	emplace_all_active();

}



void PrunableSatGraph::emplace_all_active() {

	vertStatus = VertStatusMap();
	edgeStatus = EdgeStatusMap();

	auto vertPair = boost::vertices(base);
	std::for_each(vertPair.first, vertPair.second, [this](VertDescriptor v) {
		vertStatus.emplace(v, true);
	});

	auto edgePair = boost::edges(base);
	std::for_each(edgePair.first, edgePair.second, [this](EdgeDescriptor v) {
		edgeStatus.emplace(v, true);
	});

}
