#include "problem.h"
#include <queue>
#include "boost/graph/breadth_first_search.hpp"
#include "boost/graph/connected_components.hpp"
#include "coopnet/sat/solving/formula.h"
#include "coopnet/sat/visitor/satisfiability_visitor.h"
#include "assignment.h"
#include "shuffler.h"

using namespace sat;



namespace {

	boost::dynamic_properties generate_dyn_props(
		SatGraph& prob_graph) {

		boost::dynamic_properties dyn_props;

		// Generate dynamic_properties
		auto name_map = get(&VertProp::name, prob_graph);
		dyn_props.property("name", name_map);
		auto kind_map = get(&VertProp::kind, prob_graph);
		dyn_props.property("kind", kind_map);
		auto sgn_map = get(&EdgeProp::sgn, prob_graph);
		dyn_props.property("sign", sgn_map);

		return dyn_props;

	}

	std::vector<VertDescriptor>
		calculate_connected_components(const SatGraph& prob_graph) {

		using conn_map_type = std::map<VertDescriptor, size_t>;
		auto connected_map = conn_map_type();
		auto boost_conn_map =
			boost::associative_property_map<conn_map_type>(connected_map);
		auto num_connected_components
			= boost::connected_components(prob_graph, boost_conn_map);

		auto connected_component_vertices = std::vector<VertDescriptor>();
		connected_component_vertices.reserve(num_connected_components);
		auto set_done_components = std::set<size_t>();
		for (auto vert_pair = boost::vertices(prob_graph);
			vert_pair.first != vert_pair.second; ++vert_pair.first) {

			auto component_idx = connected_map[*vert_pair.first];
			if (set_done_components.count(component_idx) == 0) {
				set_done_components.insert(component_idx);
				connected_component_vertices.emplace_back(*vert_pair.first);
			}

		}

		return std::move(connected_component_vertices);

	}

}



ClauseSatisfiability Problem::clause_satisfiability_for(
	std::shared_ptr<const Assignment> assign) const {

	auto satisfiability_collector
		= SatCollectionVisitor(*this, assign);

	auto form = Formula(*this);
	
	form.visit_problem(satisfiability_collector);
	
	return *satisfiability_collector.satisfiability;
	
}



std::shared_ptr<Assignment> Problem::create_same_sgn_assignment(bool sgn) const {

	// Should be satisfied by an assignment of all true
	auto map_assign = std::map<Node, bool>();
	for(auto pair : map_node_to_vert->left) {
		auto node = pair.first;
		map_assign.emplace(node, sgn);
	}

	auto assign = std::make_shared<sat::Assignment>();
	assign->data = std::move(map_assign);
	return assign;
	
}



void Problem::shuffle_nodes(const NodeShuffler& shuffler) {

	// Change node_vert_map to re-point the nodes
	// Need copy and swap because we can't have duplicates
	auto map_cpy = NodeVertMap();
	for (auto iter = map_node_to_vert->left.begin();
		iter != map_node_to_vert->left.end(); ++iter) {

		auto old_node = iter->first;
		auto vert = iter->second;
		auto new_node = shuffler.nodes.at(old_node.id);
		map_cpy.left.insert(std::make_pair(new_node, vert));

	}
	std::swap(map_cpy, *map_node_to_vert);

	// Rename nodes and clauses
	rename_verts(prob_graph, *map_node_to_vert);

}

void Problem::shuffle_sgns(const SgnShuffler& shuffler) {

	// Now change edge sgns
	for (auto iter = map_node_to_vert->left.begin();
		iter != map_node_to_vert->left.end(); ++iter) {

		if (!shuffler.sgns.at(iter->first.id)) {

			auto vert = iter->second;

			auto edge_pair = boost::out_edges(vert, prob_graph);
			for_each(edge_pair.first, edge_pair.second, [this](EdgeDescriptor e) {
				prob_graph[e].sgn = !prob_graph[e].sgn;
			});

		}
	}

	// Rename nodes and clauses
	rename_verts(prob_graph, *map_node_to_vert);

}
		



void Problem::build_graph(NodeList&& nodes, ClauseList&& clauses) {

	num_nodes = nodes.size();
	num_clauses = clauses.size();

	prob_graph = SatGraph();

	// Temp map to connect node-clause edges
	map_node_to_vert = std::make_shared<NodeVertMap>();

	// Add all nodes in sequence to graph
	for(auto node_to_add : nodes) {

		// Add node as vertex to graph
		auto node_vert = add_vertex(prob_graph, node_to_add);
		map_node_to_vert->insert(NodeVertMap::value_type(node_to_add, node_vert));

	}


	// Add all clauses in sequence to graph, then add all edges
	for(auto& clause_to_add : clauses) {

		// Add clause as vertex to graph
		auto clause_vert = add_vertex(prob_graph, clause_to_add);
		for(auto& lit : clause_to_add.literals()) {

			// Add node in clause as edge to graph
			//TODO: Add error handling for if node not in map.
			auto node_vert = map_node_to_vert->left.at(lit.first);
			add_edge(prob_graph, node_vert, clause_vert, lit.second);

		}

	}


	dyn_props = generate_dyn_props(prob_graph);
	connected_component_vertices = calculate_connected_components(prob_graph);
	num_connected_components = connected_component_vertices.size();
	
}





std::ostream& sat::operator<<(std::ostream& os, const Problem& prob) {

	os << "problem:" << std::endl;

	const auto& g = prob.get_graph();
	auto vert_pair = boost::vertices(prob.get_graph());
	for (auto vert = vert_pair.first; vert != vert_pair.second; ++vert) {
		const auto& prop = g[*vert];
		os << prop.kind << prop.name << std::endl;
	}

	return os << std::endl;

}
