#include "problem.h"
#include <queue>
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/graph/graph_util.h"
#include "coopnet/sat/solving/simple_formula.h"
#include "coopnet/sat/visitor/sat_collection_visitor.h"
#include "assignment.h"
#include "shuffler.h"

using namespace coopnet;



namespace {

	boost::dynamic_properties generate_dyn_props(
		BaseSatGraph& prob_graph) {

		boost::dynamic_properties dyn_props;

		// Generate dynamic_properties
		auto base_map = get(&BaseSatVProp::base, prob_graph);
		dyn_props.property("basic properties", base_map);

		return dyn_props;

	}

}



ClauseSatisfiability Problem::clause_satisfiability_for(
	std::shared_ptr<const Assignment> assign) const {

	auto satisfiability_collector
		= SatCollectionVisitor(*this, assign);

	auto form = SimpleFormula(*this);
	
	auto& g = form.graph();
	auto components = graph_util::calculate_connected_components(g);
	visit_sat_graph(satisfiability_collector, g, components.cbegin(), components.cend());
	
	return *satisfiability_collector.satisfiability;
	
}



std::shared_ptr<Assignment> Problem::create_same_sgn_assignment(bool sgn) const {

	// Should be satisfied by an assignment of all true
	auto map_assign = std::map<Node, bool>();
	for(auto pair : map_node_to_vert->left) {
		auto node = pair.first;
		map_assign.emplace(node, sgn);
	}

	auto assign = std::make_shared<coopnet::Assignment>();
	assign->data = std::move(map_assign);
	return assign;
	
}





void Problem::build_graph(NodeList&& nodes, ClauseList&& clauses) {

	num_nodes = nodes.size();
	num_clauses = clauses.size();

	prob_graph = BaseSatGraph();

	// Temp map to connect node-clause edges
	map_node_to_vert = std::make_shared<NodeVertMap>();

	// Add all nodes in sequence to graph
	for(auto node_to_add : nodes) {

		// Add node as vertex to graph
		auto prop = BaseSatGraph::vertex_property_type();
		prop.base.kind = BaseSatVProp::Node;
		prop.base.name = graph_util::node_name(node_to_add);

		auto node_vert = boost::add_vertex(prop, prob_graph);

		map_node_to_vert->insert(NodeVertMap::value_type(node_to_add, node_vert));

	}


	// Add all clauses in sequence to graph, then add all edges
	for(auto& clause_to_add : clauses) {

		// Add clause as vertex to graph
		auto prop = BaseSatGraph::vertex_property_type();
		prop.base.kind = BaseSatVProp::Clause;
		prop.base.name = graph_util::clause_name(clause_to_add);

		auto clause_vert = boost::add_vertex(prop, prob_graph);
		
		for(auto& lit : clause_to_add.literals()) {

			// Add node in clause as edge to graph
			//TODO: Add error handling for if node not in map.
			auto node_vert = map_node_to_vert->left.at(lit.first);

			auto prop = BaseSatGraph::edge_property_type();
			prop.base.sgn = lit.second;

			auto desc_pair = boost::add_edge(node_vert, clause_vert, prop, prob_graph);
			if (!desc_pair.second) std::exception("Failed to add edge to graph.");

		}

	}


	dyn_props = generate_dyn_props(prob_graph);

}





std::ostream& coopnet::operator<<(std::ostream& os, const Problem& prob) {

	os << "problem:" << std::endl;

	const auto& g = prob.get_graph();
	auto vert_pair = boost::vertices(prob.get_graph());
	for (auto vert = vert_pair.first; vert != vert_pair.second; ++vert) {
		const auto& prop = g[*vert];
		os << prop.base.kind << prop.base.name << std::endl;
	}

	return os << std::endl;

}
