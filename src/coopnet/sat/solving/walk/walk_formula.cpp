#include "walk_formula.h"
#include <iostream>
#include "coopnet/graph/base/graph_util.h"
#include "coopnet/sat/problem/problem.h"

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



WalkFormula::WalkFormula(const Problem& prob) :
	Formula<WalkProp>(prob),
	g(graph_util::create_default_concrete_graph<WalkProp>(
		prob.get_graph(), *prob.get_node_vert_translator())) {
	
	numClauses = prob.get_num_clauses();

}

WalkFormula::~WalkFormula() {

}



void WalkFormula::flip_node(VertDescriptor vertNode) {

	if (DEBUG) {

		auto oldSgn = g.graph[vertNode].node().assignment;
		auto newSgn = !oldSgn;

		std::cout << "Flip node associated with vert " << vertNode;
		std::cout << (newSgn ? " F to T" : " T to F") << std::endl;

	}

	// Flip node
	g.graph[vertNode].node().assignment = !g.graph[vertNode].node().assignment;

	// Propagate to clauses
	auto edges = boost::out_edges(vertNode, g.graph);
	for (auto edge = edges.first; edge != edges.second; ++edge) {

		auto vertClause = boost::target(*edge, g.graph);

		if (g.graph[vertNode].node().assignment == g.graph[*edge].sgn) {
			satisfy_clause(vertClause);
		} else {
			check_unsatisfied_clause(vertClause);
		}

	}

}



bool WalkFormula::is_SAT() const {

	return numClausesFailed == 0;

}



const SatGraphTranslator& WalkFormula::get_sat_graph_translator() const {
	return g.translator;
}



auto WalkFormula::create_vert_assignment() const -> VertAssignment {

	auto assignment = VertAssignment();

	auto verts = boost::vertices(g.graph);
	for (auto vert = verts.first; vert != verts.second; ++vert) {

		auto& prop = g.graph[*vert];
		if(VertKind(prop.kind) == VertKind::Node) {
			assignment.emplace(*vert, prop.node().assignment);
		}

	}

	return assignment;

}

void WalkFormula::set_vert_assignment(const VertAssignment& assignment) {

	for (auto iter = assignment.begin(); iter != assignment.end(); ++iter) {
		g.graph[iter->first].node().assignment = iter->second;
	}

	init_clause_satisfaction();

}




auto WalkFormula::graph() const -> const Graph& {
	return g.graph;
}
auto WalkFormula::graph() -> Graph& {
	return g.graph;
}

void WalkFormula::init_clause_satisfaction() {

	if (DEBUG) {
		std::cout << "Initialize clause satisfaction\n";
		std::cout << "Init nodes to Default and clauses to Unsatisfied\n";
	}

	// Init all nodes to Default and clauses to Unsatisfied
	auto verts = boost::vertices(g.graph);
	for (auto vert = verts.first; vert != verts.second; ++vert) {
		auto& prop = g.graph[*vert];
		if (VertKind(prop.kind) == VertKind::Node) {
			prop.node().walkStatus = WalkVertStatus::Default;
		} else {
			prop.clause().walkStatus = WalkVertStatus::Unsatisfied;
		}
	}

	
	if (DEBUG) std::cout << "Set satisfied clauses to Satisfied\n";

	// Set clauses to Satisfied, and init numClausesFailed
	ClauseSatisfiability satisfiability;

	auto edges = boost::edges(g.graph);
	for (auto edge = edges.first; edge != edges.second; ++edge) {

		auto vertNode = boost::source(*edge, g.graph);
		auto vertClause = boost::target(*edge, g.graph);
		if (VertKind(g.graph[vertNode].kind) == VertKind::Clause)
			std::swap(vertNode, vertClause);

		// If sign of literal in clause matches assignment, clause is satisfied
		auto sgn_of_literal = g.graph[*edge].sgn;
		auto assigned_val = g.graph[vertNode].node().assignment;

		if (sgn_of_literal == assigned_val) {
			satisfiability.clausesSatisfied.insert(vertClause);
			g.graph[vertClause].clause().walkStatus = WalkVertStatus::Satisfied;
		}

	}

	if (DEBUG) std::cout << "Calc number of clauses failed\n";

	numClausesFailed = numClauses - satisfiability.clausesSatisfied.size();

}



void WalkFormula::satisfy_clause(VertDescriptor vertClause) {

	auto& status = g.graph[vertClause].clause().walkStatus;

	// If clause is satisfied, no change
	if (status == WalkVertStatus::Unsatisfied) {
		--numClausesFailed;
		status = WalkVertStatus::Satisfied;
	}

}

void WalkFormula::check_unsatisfied_clause(VertDescriptor vertClause) {

	auto& status = g.graph[vertClause].clause().walkStatus;

	if (DEBUG) {
		if (status == WalkVertStatus::Unsatisfied)
			throw std::exception("Clause already unsatisfied; improperly called.");
	}

	// Check if no nodes/edges satisfy clause
	auto edges = boost::out_edges(vertClause, g.graph);
	if (std::none_of(edges.first, edges.second, [this](EdgeDescriptor edge) {
		auto vertNode = boost::target(edge, g.graph);
		return g.graph[vertNode].node().assignment == g.graph[edge].sgn;
	})) {
		++numClausesFailed;
		status = WalkVertStatus::Unsatisfied;
	}

}
