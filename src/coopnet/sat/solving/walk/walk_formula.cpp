#include "walk_formula.h"
#include <iostream>
#include "coopnet/graph/graph_util.h"
#include "coopnet/sat/problem/problem.h"

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



WalkFormula::WalkFormula(const Problem& prob) :
	Formula<WalkVProp, WalkEProp>(prob),
	g(graph_util::create_default_concrete_graph<WalkVProp, WalkEProp>(prob.get_graph())) {
	
	numClauses = prob.get_num_clauses();

}

WalkFormula::~WalkFormula() { }



void WalkFormula::flip_node(Node node) {

	auto vertNode = node_vert_map().left.at(node);

	if (DEBUG) {

		auto oldSgn = g[vertNode].assignment;
		auto newSgn = !oldSgn;

		std::cout << "Flip node " << node.id << " with vert " << vertNode;
		std::cout << (newSgn ? "F to T" : "T to F") << std::endl;

	}

	// Flip node
	g[vertNode].assignment = !g[vertNode].assignment;

	// Propagate to clauses
	auto edges = boost::out_edges(vertNode, g);
	for (auto edge = edges.first; edge != edges.second; ++edge) {

		auto vertClause = boost::target(*edge, g);

		if (g[vertNode].assignment == g[*edge].base.sgn) {
			satisfy_clause(vertClause);
		} else {
			check_unsatisfied_clause(vertClause);
		}

	}

}



bool WalkFormula::is_SAT() const {

	return numClausesFailed == 0;

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

	init_clause_satisfaction();

}




auto WalkFormula::graph() const -> const Graph& {
	return g;
}
auto WalkFormula::graph() -> Graph& {
	return g;
}

void WalkFormula::init_clause_satisfaction() {

	if (DEBUG) {
		std::cout << "Initialize clause satisfaction\n";
		std::cout << "Init nodes to Default and clauses to Unsatisfied\n";
	}

	// Init all nodes to Default and clauses to Unsatisfied
	auto verts = boost::vertices(g);
	for (auto vert = verts.first; vert != verts.second; ++vert) {
		auto& prop = g[*vert];
		if (prop.base.kind == BaseSatVProp::Node) {
			prop.walkStatus = WalkVertStatus::Default;
		} else {
			prop.walkStatus = WalkVertStatus::Unsatisfied;
		}
	}

	
	if (DEBUG) std::cout << "Set satisfied clauses to Satisfied\n";

	// Set clauses to Satisfied, and init numClausesFailed
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
			g[vertClause].walkStatus = WalkVertStatus::Satisfied;
		}

	}

	if (DEBUG) std::cout << "Calc number of clauses failed\n";

	numClausesFailed = numClauses - satisfiability.clausesSatisfied.size();

}



void WalkFormula::satisfy_clause(VertDescriptor vertClause) {

	auto& status = g[vertClause].walkStatus;

	// If clause is satisfied, no change
	if (status == WalkVertStatus::Unsatisfied) {
		--numClausesFailed;
		status = WalkVertStatus::Satisfied;
	}

}

void WalkFormula::check_unsatisfied_clause(VertDescriptor vertClause) {

	auto& status = g[vertClause].walkStatus;

	if (DEBUG) {
		if (status == WalkVertStatus::Unsatisfied)
			throw std::exception("Clause already unsatisfied; improperly called.");
	}

	// Check if no nodes/edges satisfy clause
	auto edges = boost::out_edges(vertClause, g);
	if (std::none_of(edges.first, edges.second, [this](EdgeDescriptor edge) {
		auto vertNode = boost::target(edge, g);
		return g[vertNode].assignment == g[edge].base.sgn;
	})) {
		++numClausesFailed;
		status = WalkVertStatus::Unsatisfied;
	}

}
