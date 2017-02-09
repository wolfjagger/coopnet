#include "literal_shuffler.h"
#include <forward_list>
#include <numeric>
#include "alphali/util/random.h"
#include "problem.h"
#include "assignment.h"

using namespace sat;



literal_shuffler::literal_shuffler(const problem& prob) {

	literals = std::vector<literal>();
	literals.reserve(prob.get_num_nodes());
	for (auto i = 0; i < prob.get_num_nodes(); ++i) {
		literals.emplace_back(i, true);
	}

	map_node_to_vert = &prob.get_node_vert_map();

	shuffle();

}



void literal_shuffler::shuffle() {

	alphali::shuffle(literals.begin(), literals.end());

	for_each(literals.begin(), literals.end(), [](literal& lit) {
		if (alphali::coin_flip()) lit.sgn = !lit.sgn;
	});

}



literal literal_shuffler::shuffle_literal(const literal& lit) const {

	auto new_lit = literals.at(lit.n.id);
	new_lit.sgn = (lit.sgn == new_lit.sgn);
	return lit;

}

void literal_shuffler::apply_to_assignment(assignment& assign) const {

	for_each(literals.cbegin(), literals.cend(), [this, &assign](const literal& lit) {
		auto& sgn = assign.data.at(lit.n);
		sgn = (sgn == lit.sgn);
	});

}

void literal_shuffler::apply_to_problem(problem& prob) const {

	prob.apply_shuffle(*this);

}
