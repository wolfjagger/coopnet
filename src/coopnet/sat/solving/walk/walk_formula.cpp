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

	numClauses = prob.get_num_clauses();
	numClausesFailed = std::make_shared<size_t>();
	walkVisitor = std::make_unique<BfsWalkVisitor>(numClausesFailed);

}

WalkFormula::~WalkFormula() { }



void WalkFormula::flip_node(Node node) {

	auto vertNode = node_vert_map().left.at(node);

	graph()[vertNode].walkStatus = WalkVertStatus::Flip;

	if (DEBUG) {

		auto oldSgn = graph()[vertNode].assignment;
		auto newSgn = !oldSgn;

		std::cout << "Flip node " << node.id << " with vert " << vertNode;
		std::cout << (newSgn ? "F to T" : "T to F") << std::endl;

	}

	boost::breadth_first_visit(
		graph(), vertNode, greyBuffer,
		*walkVisitor, colorPropMap);

}



bool WalkFormula::is_SAT() const {

	return *numClausesFailed == 0;

}

Assignment WalkFormula::create_assignment() const {

	Assignment assignment;
	auto copyPred = [this, &assignment](auto pair) {
		assignment.data.emplace(pair.first, g[pair.second].assignment);
	};
	apply_to_node_vert_map(copyPred);

	return assignment;

}

void WalkFormula::set_assignment(const Assignment& assignment) {

	auto copyPred = [this, &assignment](auto pair) {
		g[pair.second].assignment = assignment.data.at(pair.first);
	};
	apply_to_node_vert_map(copyPred);

	init_count_clauses_failed();

}




auto WalkFormula::graph() const -> const Graph& {
	return g;
}
auto WalkFormula::graph() -> Graph& {
	return g;
}

void WalkFormula::init_count_clauses_failed() {
	
	ClauseSatisfiability satisfiability;

	auto edges = boost::edges(g);
	for (auto edge = edges.first; edge != edges.second; ++edge) {

		auto vertNode = boost::source(*edge, g);
		auto vertClause = boost::target(*edge, g);
		if (g[vertNode].base.kind == BaseSatVProp::Clause)
			std::swap(vertNode, vertClause);

		// If sign of literal in clause matches assignment, clause is satisfied
		auto n = node_vert_map().right.at(vertNode);
		auto sgn_of_literal = g[*edge].base.sgn;
		auto assigned_val = g[vertNode].assignment;
		if (sgn_of_literal == assigned_val) {
			satisfiability.clausesSatisfied.insert(vertClause);
		}

	}

	*numClausesFailed = numClauses - satisfiability.clausesSatisfied.size();

}
