#include "formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



Formula::Formula(const Problem& prob) :
	prob(std::cref(prob)),
	pruneGraph(prob.get_graph()) {

}



void Formula::reverse_prune_to_assignment(Node n) {

	auto vert = node_vert_map().left.at(n);
	pruneGraph.reverse_to_vert(vert);

}



const NodeVertMap& Formula::node_vert_map() const {
	return *node_vert_map_ptr();
}
std::shared_ptr<const NodeVertMap> Formula::node_vert_map_ptr() const {
	return prob.get().get_node_vert_map();
}
		
const PrunableSatGraph& Formula::prune_graph() const {
	return pruneGraph;
}



bool Formula::is_SAT() const {
	return !pruneGraph.is_indeterminate();
}

const BaseSatGraph& Formula::prob_graph() const {
	return prob.get().get_graph();
}



Assignment Formula::create_assignment() const {

	if (pruneGraph.is_indeterminate()) {
		throw std::exception("Incomplete assignment cannot be transformed.");
	}

	Assignment assign;
	auto copy_pred = [this, &assign](auto pair) {
		assign.data.emplace(pair.first, bool(pruneGraph.get_assignment(pair.second)));
	};

	auto& node_vert_map = prob.get().get_node_vert_map()->left;
	std::for_each(node_vert_map.begin(), node_vert_map.end(), copy_pred);

	return assign;

}
