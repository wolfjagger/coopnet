#include "node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "coopnet/sat/solving/dpll/dpll_formula.h"



using namespace sat;



boost::optional<node> node_chooser::choose(const formula& form) {

	auto& assign = form.get_incomplete_assignment();
	auto& assign_map = assign.data;

	// If none, return no node
	if (std::none_of(assign_map.cbegin(), assign_map.cend(), is_ind_pair)) {
		return boost::optional<node>();
	} else {

		auto vert = do_choose(assign_map);

		return assign.node_to_vertex_map->right.at(vert);

	}

}





vertex_descriptor next_node_chooser::do_choose(const assignment_map& assign_map) {

	auto iter = std::find_if(
		assign_map.cbegin(), assign_map.cend(), is_ind_pair);
	return iter->first;

}

vertex_descriptor last_node_chooser::do_choose(const assignment_map& assign_map) {

	auto iter = std::find_if(
		assign_map.crbegin(), assign_map.crend(), is_ind_pair);
	return iter->first;

}



namespace {
	auto rand_engine = std::mt19937_64(std::random_device()());
}


vertex_descriptor rand_node_chooser::do_choose(const assignment_map& assign_map) {

	auto iter = alphali::random_find_if(
		assign_map.cbegin(), assign_map.cend(), is_ind_pair, rand_engine);
	return iter->first;

}
