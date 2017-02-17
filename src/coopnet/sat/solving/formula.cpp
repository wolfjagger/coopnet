#include "formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



formula::formula(const problem& prob) :
	prob_graph(std::cref(prob.get_graph())),
	partial_assign(prob) {

}

formula::~formula() {

}



bool formula::is_SAT() const {

	return std::none_of(
		partial_assign.data.cbegin(), partial_assign.data.cend(),
		is_ind_pair);

}
