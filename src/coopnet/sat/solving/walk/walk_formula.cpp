#include "walk_formula.h"
#include <iostream>
#include "coopnet/graph/graph_util.h"
#include "coopnet/sat/problem/problem.h"
#include "walk_visitor.h"

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



WalkFormula::WalkFormula(const Problem& prob) :
	Formula<WalkVProp, WalkEProp>(prob),
	g(graph_util::create_default_concrete_graph<WalkVProp, WalkEProp>(prob.get_graph())),
	greyBuffer() {
	
	colorPropMap = boost::get(&WalkVProp::color, graph());

	numClausesFailed = std::make_shared<unsigned int>();
	walkVisitor = std::make_unique<BfsWalkVisitor>(numClausesFailed);

}

WalkFormula::~WalkFormula() { }



void WalkFormula::flip_node(Node node) {

	auto vert_node = node_vert_map().left.at(node);

	graph()[vert_node].walkStatus = WalkVertStatus::Flip;

	if (DEBUG) {

		auto old_sgn = graph()[vert_node].assignment;
		auto new_sgn = !old_sgn;

		std::cout << "Flip node " << node.id << " with vert " << vert_node;
		std::cout << (new_sgn ? "F to T" : "T to F") << std::endl;

	}

	boost::breadth_first_visit(
		graph(), vert_node, greyBuffer,
		*walkVisitor, colorPropMap);

}



bool WalkFormula::is_SAT() const {

	return *numClausesFailed == 0;

}

Assignment WalkFormula::create_assignment() const {

	Assignment assignment;
	auto copy_pred = [this, &assignment](auto pair) {
		assignment.data.emplace(pair.first, g[pair.second].assignment);
	};
	apply_to_node_vert_map(copy_pred);

	return assignment;

}

void WalkFormula::set_assignment(const Assignment& assignment) {

	auto copy_pred = [this, &assignment](auto pair) {
		g[pair.second].assignment = assignment.data.at(pair.first);
	};
	apply_to_node_vert_map(copy_pred);

}




auto WalkFormula::graph() const -> const Graph& {
	return g;
}
auto WalkFormula::graph() -> Graph& {
	return g;
}
