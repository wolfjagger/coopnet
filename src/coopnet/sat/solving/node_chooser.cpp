#include "node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "alphali/util/random.h"
#include "coopnet/sat/solving/dpll/dpll_formula.h"



using namespace coopnet;



boost::optional<NodeChoice> NodeChooser::choose(const Formula& form) {

	auto& pruneGraph = form.prune_graph();

	// If none, return no node
	if(!pruneGraph.prune_info().is_indeterminate()) {
		return boost::optional<NodeChoice>();
	} else {

		auto choice = do_choose(form);

		auto n = form.node_vert_map().right.at(choice.first);

		return NodeChoice{ n, choice.second };

	}

}

bool NodeChooser::is_ind_pair(std::pair<VertDescriptor, boost::tribool> pair) {
	return boost::indeterminate(pair.second);
}





auto NextNodeChooser::do_choose(const Formula& form)
	-> VertChoice {

	auto& pruneGraph = form.prune_graph();
	auto& assign_map = pruneGraph.prune_info().get_assignment_map();

	auto iter = std::find_if(
		assign_map.cbegin(), assign_map.cend(), is_ind_pair);
	return std::make_pair(iter->first, true);

}



namespace {
	auto rand_engine = std::mt19937_64(std::random_device()());
}


auto RandNodeChooser::do_choose(const Formula& form)
	-> VertChoice {

	auto& pruneGraph = form.prune_graph();
	auto& assign_map = pruneGraph.prune_info().get_assignment_map();

	auto iter = alphali::random_find_if(
		assign_map.cbegin(), assign_map.cend(), is_ind_pair, rand_engine);
	return std::make_pair(iter->first, alphali::coin_flip());

}
