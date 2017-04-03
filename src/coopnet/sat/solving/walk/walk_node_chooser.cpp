#include "walk_node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "walk_formula.h"

using namespace coopnet;



VertDescriptor WalkNodeChooser::choose(const WalkFormula& form) {

	return do_choose(form);

}

std::unique_ptr<WalkNodeChooser> WalkNodeChooser::create(WalkNodeChoiceMode mode) {

	switch (mode) {
	case WalkNodeChoiceMode::Random:
		return std::make_unique<RandWalkNodeChooser>();
	default:
		throw std::exception("Unknown walk node choice mode.");
	}

}




namespace detail {
	static auto rand_node_engine = std::mt19937_64(std::random_device()());
}

VertDescriptor RandWalkNodeChooser::do_choose(const WalkFormula& form) {

	auto& graph = form.graph();

	auto vPair = boost::vertices(graph);

	auto iter = alphali::random_find_if(vPair.first, vPair.second,
		[&graph](VertDescriptor v) {

		return true;

	}, detail::rand_node_engine);

	return *iter;

}
