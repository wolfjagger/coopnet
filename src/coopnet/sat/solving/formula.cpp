#include "formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



formula::formula(const problem& prob) :
	prob(std::cref(prob)),
	partial_assign(prob) {

}



bool formula::is_SAT() const {

	return std::none_of(
		partial_assign.data.cbegin(), partial_assign.data.cend(),
		is_ind_pair);

}

const graph& formula::get_prob_graph() const {
	return prob.get().get_graph();
}

const std::vector<vertex_descriptor>&
formula::get_prob_connected_component_entry_pts() const {
	return prob.get().connected_component_entry_pts();
}
