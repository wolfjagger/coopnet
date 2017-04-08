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
			sat_clause(vertNode, vertClause);
		} else {
			unsat_clause(vertNode, vertClause);
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
		if(prop.kind() == VertKind::Node) {
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

	ClauseSatisfiability satisfiability;

	// Init all clause satisfaction and node breakCount.
	auto verts = boost::vertices(g.graph);
	for (auto vert = verts.first; vert != verts.second; ++vert) {

		auto& prop = g.graph[*vert];
		if (prop.kind() == VertKind::Clause) {

			auto lamSatEdge = [this](EdgeDescriptor e) {
				auto node = boost::target(e, g.graph);
				return g.graph[e].sgn == g.graph[node].node().assignment;
			};

			auto edges = boost::out_edges(*vert, g.graph);
			auto numSat = std::count_if(edges.first, edges.second, lamSatEdge);
			prop.clause().numSat = unsigned int(numSat);

			// If no nodes satisfy, flipping any will change the clause
			if (numSat == 0) {

				std::for_each(edges.first, edges.second, [this](EdgeDescriptor e) {
					auto node = boost::target(e, g.graph);
					++g.graph[node].node().breakCount;
				});

			} else {
				
				// If exactly one node satisfies, flipping (only) it will change the clause
				// If more nodes satisfy, flipping any node cannot change satisfaction
				if (numSat == 1) {
					auto edgeSat = std::find_if(edges.first, edges.second, lamSatEdge);
					--g.graph[boost::target(*edgeSat, g.graph)].node().breakCount;
				}

				satisfiability.clausesSatisfied.insert(*vert);

			}

		}

	}

	if (DEBUG) std::cout << "Calc number of clauses failed\n";

	numClausesFailed = numClauses - satisfiability.clausesSatisfied.size();

}



namespace {

	// sat_clause and unsat_clause are opposites, so reduce redundancy with this
	template<typename Graph>
	void change_sat_clause(
		Graph& graph, size_t& numClausesFailed,
		VertDescriptor changeNode, VertDescriptor clause, bool toSat) {

		auto baseNumSat = toSat ? 0 : 1;
		auto baseChange = toSat ? 1 : -1;

		auto numSat = graph[clause].clause().numSat;

		// If changing from 0 to 1 sat or 1 to 0 sat, then
		//  update all breakCounts, update changeNode twice
		//  and change numClausesFailed
		if (numSat == baseNumSat) {

			auto edges = boost::out_edges(clause, graph);
			std::for_each(edges.first, edges.second,
				[&graph, changeNode, baseChange](EdgeDescriptor e) {

				auto node = boost::target(e, graph);
				graph[node].node().breakCount -= baseChange;

				// Extra since flipping node would unsat clause
				if (node == changeNode)
					graph[node].node().breakCount -= baseChange;

			});

			numClausesFailed -= baseChange;

		}
		// If changing from 1 to 2 sat or 2 to 1 sat,
		//  update only currently satisfying breakCount
		else if (numSat == baseNumSat + 1) {

			auto edges = boost::out_edges(clause, graph);
			auto edgeOtherSat = std::find_if(edges.first, edges.second,
				[&graph, changeNode](EdgeDescriptor e) {

				auto node = boost::target(e, graph);
				return (node != changeNode &&
					graph[e].sgn == graph[node].node().assignment);

			});

			graph[boost::target(*edgeOtherSat, graph)].node().breakCount
				+= baseChange;

		}

		// Change numSat
		graph[clause].clause().numSat += baseChange;

	}

}

void WalkFormula::sat_clause(
	VertDescriptor satNode, VertDescriptor vertClause) {

	change_sat_clause(g.graph, numClausesFailed, satNode, vertClause, true);

}



void WalkFormula::unsat_clause(
	VertDescriptor unsatNode, VertDescriptor vertClause) {

	change_sat_clause(g.graph, numClausesFailed, unsatNode, vertClause, false);

}
