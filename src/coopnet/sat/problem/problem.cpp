#include "problem.h"
#include <queue>
#include "boost/property_map/transform_value_property_map.hpp"
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/graph/graph_util.h"
#include "coopnet/sat/solving/simple_formula.h"
#include "coopnet/sat/visitor/sat_collection_visitor.h"
#include "assignment.h"
#include "shuffler.h"

using namespace coopnet;



namespace {

	boost::dynamic_properties generate_dyn_props(
		BaseSatGraph& probGraph) {

		boost::dynamic_properties dynProps;

		// Generate dynamic_properties

		auto baseVertMap = get(&BaseSatVProp::base, probGraph);

		auto kindLam = [](BaseSatVProp::Base& base) { return base.kind; };
		auto kindMap = boost::transform_value_property_map<decltype(kindLam), decltype(baseVertMap)>(
			kindLam, baseVertMap);
		dynProps.property("Kind", kindMap);

		auto nameLam = [](BaseSatVProp::Base& base) { return base.name; };
		auto nameMap = boost::transform_value_property_map<decltype(nameLam), decltype(baseVertMap)>(
			nameLam, baseVertMap);
		dynProps.property("Name", nameMap);


		auto baseEdgeMap = get(&BaseSatEProp::base, probGraph);

		auto sgnLam = [](BaseSatEProp::Base& base) { return base.sgn; };
		auto sgnMap = boost::transform_value_property_map<decltype(sgnLam), decltype(baseEdgeMap)>(
			sgnLam, baseEdgeMap);
		dynProps.property("Sign", sgnMap);

		return dynProps;

	}

}



ClauseSatisfiability Problem::clause_satisfiability_for(
	std::shared_ptr<const Assignment> assign) const {

	auto satisfiabilityCollector
		= SatCollectionVisitor(*this, assign);

	auto form = SimpleFormula(*this);
	
	auto& g = form.graph();
	auto components = graph_util::calculate_connected_components(g);
	visit_sat_graph(g, satisfiabilityCollector, components.cbegin(), components.cend());
	
	return *satisfiabilityCollector.satisfiability;
	
}



std::shared_ptr<Assignment> Problem::create_same_sgn_assignment(bool sgn) const {

	// Should be satisfied by an assignment of all true
	auto mapAssign = std::map<Node, bool>();
	for(auto pair : mapNodeToVert->left) {
		auto node = pair.first;
		mapAssign.emplace(node, sgn);
	}

	auto assign = std::make_shared<coopnet::Assignment>();
	assign->data = std::move(mapAssign);
	return assign;
	
}



NodeList Problem::generate_node_list() const {

	auto list = NodeList();
	for (auto iter = mapNodeToVert->left.begin(); iter != mapNodeToVert->left.end(); ++iter) {
		list.push_back(iter->first);
	}

	return list;

}





void Problem::build_graph(NodeList&& nodes, ClauseList&& clauses) {

	numNodes = nodes.size();
	numClauses = clauses.size();

	probGraph = BaseSatGraph();

	// Temp map to connect node-clause edges
	mapNodeToVert = std::make_shared<NodeVertMap>();

	// Add all nodes in sequence to graph
	for(auto node_to_add : nodes) {

		// Add node as vertex to graph
		auto prop = BaseSatGraph::vertex_property_type();
		prop.base.kind = BaseSatVProp::Node;
		prop.base.name = graph_util::node_name(node_to_add);

		auto nodeVert = boost::add_vertex(prop, probGraph);

		mapNodeToVert->insert(NodeVertMap::value_type(node_to_add, nodeVert));

	}


	// Add all clauses in sequence to graph, then add all edges
	for(auto& clause_to_add : clauses) {

		// Add clause as vertex to graph
		auto prop = BaseSatGraph::vertex_property_type();
		prop.base.kind = BaseSatVProp::Clause;
		prop.base.name = graph_util::clause_name(clause_to_add);

		auto clauseVert = boost::add_vertex(prop, probGraph);
		
		for(auto& lit : clause_to_add.literals()) {

			// Add node in clause as edge to graph
			//TODO: Add error handling for if node not in map.
			auto nodeVert = mapNodeToVert->left.at(lit.first);

			auto prop = BaseSatGraph::edge_property_type();
			prop.base.sgn = lit.second;

			auto desc_pair = boost::add_edge(nodeVert, clauseVert, prop, probGraph);
			if (!desc_pair.second) std::exception("Failed to add edge to graph.");

		}

	}


	dynProps = generate_dyn_props(probGraph);

}





std::ostream& coopnet::operator<<(std::ostream& os, const Problem& prob) {

	os << "problem:" << std::endl;

	const auto& g = prob.get_graph();
	auto vertPair = boost::vertices(prob.get_graph());
	for (auto vert = vertPair.first; vert != vertPair.second; ++vert) {
		const auto& prop = g[*vert];
		os << prop.base.kind << prop.base.name << std::endl;
	}

	return os << std::endl;

}
