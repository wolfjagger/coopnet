#include "walk_node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "walk_formula.h"

using namespace coopnet;



WalkNodeChooser::WalkNodeChooser(const WalkFormula& initForm) :
	form(initForm) {

}

std::unique_ptr<WalkNodeChooser>
WalkNodeChooser::create(const WalkFormula& form, WalkNodeChoiceMode mode) {

	switch (mode) {
	case WalkNodeChoiceMode::Random:
		return std::make_unique<RandWalkNodeChooser>(form);
	case WalkNodeChoiceMode::GSAT:
		return std::make_unique<GSATNodeChooser>(form);
	default:
		throw std::exception("Unknown walk node choice mode.");
	}

}

VertDescriptor WalkNodeChooser::choose() {

	return do_choose();

}



RandWalkNodeChooser::RandWalkNodeChooser(const WalkFormula& initForm) :
	WalkNodeChooser(initForm) {

}

namespace {
	static auto rand_node_engine = std::mt19937_64(std::random_device()());
}

VertDescriptor RandWalkNodeChooser::do_choose() {

	auto& graph = form.graph();

	auto vPair = boost::vertices(graph);

	auto iter = alphali::random_find_if(vPair.first, vPair.second,
		[&graph](VertDescriptor v) {

		return VertKind(graph[v].kind) == VertKind::Node;

	}, rand_node_engine);

	return *iter;

}





namespace {

	template<typename Graph>
	void sort_nodes(Graph& g, std::vector<VertDescriptor>& nodes) {

		std::sort(nodes.begin(), nodes.end(), [&g](VertDescriptor v1, VertDescriptor v2) {
			return (g[v1].node().breakCount > g[v2].node().breakCount);
		});

	}

	std::vector<VertDescriptor> init_sorted_nodes(const WalkFormula& form) {

		auto nodes = std::vector<VertDescriptor>();

		auto& g = form.graph();
		auto verts = boost::vertices(g);
		std::for_each(verts.first, verts.second, [&g, &nodes](VertDescriptor v) {
			if (VertKind(g[v].kind) == VertKind::Node) nodes.push_back(v);
		});

		sort_nodes(g, nodes);

		return nodes;

	}

}

GSATNodeChooser::GSATNodeChooser(const WalkFormula& initForm) :
	WalkNodeChooser(initForm),
	sortedNodes(init_sorted_nodes(initForm)) {

}



VertDescriptor GSATNodeChooser::do_choose() {

	auto& g = form.graph();

	sort_nodes(g, sortedNodes);

	auto begin_same = sortedNodes.begin();
	auto maxBreakCount = g[*begin_same].node().breakCount;
	auto end_same = std::find_if(sortedNodes.begin(), sortedNodes.end(),
		[&g, maxBreakCount](VertDescriptor v) {
		return (g[v].node().breakCount != maxBreakCount);
	});

	auto node = alphali::random_find_if(begin_same, end_same,
		[](VertDescriptor) { return true; }, rand_node_engine);
	
	return *node;

}



UnsatClauseMCNodeChooser::UnsatClauseMCNodeChooser(
	const WalkFormula& initForm, double initGreedyProb) :
	WalkNodeChooser(initForm),
	greedyProb(initGreedyProb) {

}

VertDescriptor UnsatClauseMCNodeChooser::do_choose() {
	return 0;
}
