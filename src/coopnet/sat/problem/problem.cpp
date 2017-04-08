#include "problem.h"
#include <queue>
#include "boost/property_map/transform_value_property_map.hpp"
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/graph/base/graph_util.h"
#include "coopnet/sat/solving/simple_formula.h"
#include "coopnet/sat/visitor/sat_collection_visitor.h"
#include "assignment.h"
#include "shuffler.h"

using namespace coopnet;



namespace {

	constexpr bool DEBUG = false;

	boost::dynamic_properties generate_dyn_props(
		BaseSatGraph& probGraph) {

		boost::dynamic_properties dynProps;

		// Generate dynamic_properties

		// If you need to access a substructure, use transform_value_property_map:
		/*
		auto baseVertMap = get(&BaseSatProp::base, probGraph);
		auto kindLam = [](VProp<BaseSatProp>& vProp) { return vProp.kind; };
		auto kindMap = boost::transform_value_property_map<decltype(kindLam), decltype(baseVertMap)>(
			kindLam, baseVertMap);
		*/

		auto kindMap = get(&VProp<BaseSatProp>::kindInt, probGraph);
		dynProps.property("Kind", kindMap);

		auto nameMap = get(&VProp<BaseSatProp>::name, probGraph);
		dynProps.property("Name", nameMap);


		auto sgnMap = get(&EProp<BaseSatProp>::sgn, probGraph);
		dynProps.property("Sign", sgnMap);

		return dynProps;

	}

}



ClauseSatisfiability Problem::clause_satisfiability_for(
	std::shared_ptr<const Assignment> assign) const {

	if (DEBUG) std::cout << "Create satisfiability collector\n";

	auto satisfiabilityCollector
		= SatCollectionVisitor(*this, assign);

	if (DEBUG) std::cout << "Calculate connected components\n";

	auto components = graph_util::calculate_connected_components(probGraph);

	if (DEBUG) std::cout << "Visit with satisfiability collector\n";

	visit_sat_graph(probGraph, satisfiabilityCollector, components.cbegin(), components.cend());
	
	if (DEBUG) std::cout << "Return satisfiability\n";

	return *satisfiabilityCollector.satisfiability;
	
}



std::shared_ptr<Assignment> Problem::create_same_sgn_assignment(bool sgn) const {

	if (DEBUG) std::cout << "Create same sgn assignment\n";

	// Should be satisfied by an assignment of all true
	auto mapAssign = std::map<Node, bool>();
	for(auto node = translator->node_begin();
		node != translator->node_end(); ++node) {

		mapAssign.emplace(*node, sgn);

	}

	auto assign = std::make_shared<coopnet::Assignment>();
	assign->data = std::move(mapAssign);
	return assign;
	
}





void Problem::build_graph(NodeList&& nodes, ClauseList&& clauses) {

	if (DEBUG) std::cout << "Build graph\n";

	numNodes = nodes.size();
	numClauses = clauses.size();

	probGraph = BaseSatGraph();

	// Temp map to connect node-clause edges
	auto mapNodeToVert = NodeVertMap();

	if (DEBUG) std::cout << "Add nodes\n";

	// Add all nodes in sequence to graph
	for(auto node_to_add : nodes) {

		// Add node as vertex to graph
		auto prop = BaseSatGraph::vertex_property_type(BaseSatProp::Node());
		prop.name = graph_util::node_name(node_to_add);

		auto nodeVert = boost::add_vertex(prop, probGraph);

		mapNodeToVert.insert(NodeVertMap::value_type(node_to_add, nodeVert));

	}

	if (DEBUG) std::cout << "Create node-vert translator\n";

	translator = std::make_shared<SatGraphTranslator>(std::move(mapNodeToVert));


	if (DEBUG) std::cout << "Add clauses\n";

	// Add all clauses in sequence to graph, then add all edges
	for(auto& clause_to_add : clauses) {

		// Add clause as vertex to graph
		auto prop = BaseSatGraph::vertex_property_type(BaseSatProp::Clause());
		prop.name = graph_util::clause_name(clause_to_add);

		auto clauseVert = boost::add_vertex(prop, probGraph);
		
		for(auto& lit : clause_to_add.literals()) {

			// Add node in clause as edge to graph
			//TODO: Add error handling for if node not in map.
			auto nodeVert = translator->node_to_vert(lit.first);

			auto prop = BaseSatGraph::edge_property_type();
			prop.sgn = lit.second;

			auto desc_pair = boost::add_edge(nodeVert, clauseVert, prop, probGraph);
			if (!desc_pair.second) std::exception("Failed to add edge to graph.");

		}

	}


	if (DEBUG) std::cout << "Generate dynamic properties\n";

	dynProps = generate_dyn_props(probGraph);

}





std::ostream& coopnet::operator<<(std::ostream& os, const Problem& prob) {

	os << "problem:" << std::endl;

	const auto& g = prob.get_graph();
	auto vertPair = boost::vertices(prob.get_graph());
	for (auto vert = vertPair.first; vert != vertPair.second; ++vert) {
		const auto& prop = g[*vert];
		os << prop.kind() << prop.name << std::endl;
	}

	return os << std::endl;

}
