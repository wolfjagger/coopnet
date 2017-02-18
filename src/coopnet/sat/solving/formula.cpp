#include "formula.h"
#include <iostream>

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



formula::formula(const problem& prob) :
	prob(std::cref(prob)),
	partial_assign(prob) {

}

formula::~formula() {

}



const graph& formula::get_prob_graph() const {
	return prob.get().get_graph();
}

bool formula::is_SAT() const {

	return std::none_of(
		partial_assign.data.cbegin(), partial_assign.data.cend(),
		is_ind_pair);

}
