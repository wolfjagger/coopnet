#include "node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "alphali/util/random.h"
#include "coopnet/sat/solving/dpll/dpll_formula.h"



using namespace sat;



boost::optional<NodeChoice> NodeChooser::choose(const Formula& form) {

	auto& assign = form.get_incomplete_assignment();
	auto& assign_map = assign.data;

	// If none, return no node
	if (std::none_of(assign_map.cbegin(), assign_map.cend(), is_ind_pair)) {
		return boost::optional<NodeChoice>();
	} else {

		auto choice = do_choose(form, assign_map);

		auto n = assign.node_to_vertex_map->right.at(choice.first);

		return NodeChoice{ n, choice.second };

	}

}





auto NextNodeChooser::do_choose(
	const Formula& form, const AssignmentMap& assign_map)
	-> VertChoice {

	auto iter = std::find_if(
		assign_map.cbegin(), assign_map.cend(), is_ind_pair);
	return std::make_pair(iter->first, true);

}

auto LastNodeChooser::do_choose(
	const Formula& form, const AssignmentMap& assign_map)
	-> VertChoice {

	auto iter = std::find_if(
		assign_map.crbegin(), assign_map.crend(), is_ind_pair);
	return std::make_pair(iter->first, false);

}



namespace {
	auto rand_engine = std::mt19937_64(std::random_device()());
}


auto RandNodeChooser::do_choose(
	const Formula& form, const AssignmentMap& assign_map)
	-> VertChoice {

	auto iter = alphali::random_find_if(
		assign_map.cbegin(), assign_map.cend(), is_ind_pair, rand_engine);
	return std::make_pair(iter->first, alphali::coin_flip());

}
