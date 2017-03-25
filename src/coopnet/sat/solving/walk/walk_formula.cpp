#include "walk_formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}


WalkFormula::WalkFormula(const Problem& prob) :
	Formula<WalkVProp, WalkEProp>(prob),
	greyBuffer() {

	walkVisitor = std::make_unique<WalkVisitor>();

}



void WalkFormula::flip_node(Node node) {

	auto vert_node = node_vert_map().left.at(node);

	graph()[vert_node].walk.status = WalkVertStatus::Flip;

	if (DEBUG) {

		auto old_sgn = graph()[vert_node].walk.assignment;
		auto new_sgn = !old_sgn;

		std::cout << "Flip node " << node.id << " with vert " << vert_node;
		std::cout << (new_sgn ? "F to T" : "T to F") << std::endl;

	}

	boost::breadth_first_visit(
		graph(), vert_node, greyBuffer,
		*walkVisitor, colorPropMap);

}

bool WalkFormula::is_SAT_cached() const {

	return walkVisitor.num_failed == 0;

}