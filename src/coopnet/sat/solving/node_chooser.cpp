#include "node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "alphali/util/random.h"
#include "coopnet/sat/solving/dpll/dpll_formula.h"



using namespace coopnet;



boost::optional<NodeChoice> NodeChooser::choose(const Formula& form) {

	auto& pruneGraph = form.prune_graph();

	// If none, return no node
	if(!pruneGraph.is_indeterminate()) {
		return boost::optional<NodeChoice>();
	} else {

		auto choice = do_choose(form);

		auto n = form.node_vert_map().right.at(choice.first);

		return NodeChoice{ n, choice.second };

	}

}





auto NextNodeChooser::do_choose(const Formula& form)
	-> VertChoice {

	auto& pruneGraph = form.prune_graph();
	
	auto vPair = boost::vertices(pruneGraph.get_graph());

	auto iter = std::find_if(vPair.first, vPair.second,
		[&pruneGraph](VertDescriptor v) {
	
		return pruneGraph.is_indeterminate_node(v);

	});

	return std::make_pair(*iter, true);

}



namespace {
	auto rand_engine = std::mt19937_64(std::random_device()());
}


auto RandNodeChooser::do_choose(const Formula& form)
	-> VertChoice {

	auto& pruneGraph = form.prune_graph();

	auto vPair = boost::vertices(pruneGraph.get_graph());

	auto iter = alphali::random_find_if(vPair.first, vPair.second,
		[&pruneGraph](VertDescriptor v) {

		return pruneGraph.is_indeterminate_node(v);

	}, rand_engine);

	return std::make_pair(*iter, true);

}
