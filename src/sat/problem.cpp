#include "problem.h"
#include "component/assignment.h"
#include "solving/satisfiability_visitor.h"
#include "boost/graph/breadth_first_search.hpp"
#include "boost/graph/connected_components.hpp"

using namespace sat;



clause_satisfiability problem::clause_satisfiability_for(
	std::shared_ptr<const assignment> assign) const {

	auto satisfiability_collector = collect_satisfiability_visitor(assign);
	auto bfs_visitor = boost::make_bfs_visitor(satisfiability_collector);
	
	for (auto source_vert : connected_component_vertices) {

		boost::breadth_first_search(
			prob_graph, boost::vertex(source_vert, prob_graph),
			boost::visitor(bfs_visitor));

	}

	return *satisfiability_collector.satisfiability;
	
}



void problem::build_graph(node_list&& nodes, clause_list&& clauses) {

	prob_graph = graph();

	// Temp map to connect node-clause edges
	map_node_to_vert = std::map<node, vertex_descriptor>();

	// Add all nodes in sequence to graph
	for(auto node_to_add : nodes) {

		// Add node as vertex to graph
		auto node_vert = add_vertex(prob_graph, node_to_add);

		map_node_to_vert.emplace(node_to_add, node_vert);

	}


	// Add all clauses in sequence to graph, then add all edges
	for(auto& clause_to_add : clauses) {

		// Add clause as vertex to graph
		auto clause_vert = add_vertex(prob_graph, clause_to_add);

		//for (auto node : clause_to_add.nodes()) {
		for(auto i=0; i<clause_to_add.size(); ++i) {

			//TODO: Add error handling for if node not in map.
			auto node = clause_to_add.nodes()[i];
			auto sgn = clause_to_add.sgns()[i];
			
			// Add node in clause as edge to graph
			auto node_vert = map_node_to_vert.at(node);
			add_edge(prob_graph, node_vert, clause_vert, sgn);

		}

	}

	// Generate dynamic_properties
	auto name_map = get(&vert_prop::name, prob_graph);
	dyn_props.property("name", name_map);
	auto kind_map = get(&vert_prop::kind, prob_graph);
	dyn_props.property("kind", kind_map);
	auto sgn_map = get(&edge_prop::sgn, prob_graph);
	dyn_props.property("sign", sgn_map);


	// Calculate connected components map and number
	using conn_map_type = std::map<vertex_descriptor, size_t>; 
	auto connected_map = conn_map_type();
	auto boost_conn_map =
		boost::associative_property_map<conn_map_type>(connected_map);
	num_connected_components =
		boost::connected_components(prob_graph, boost_conn_map);
	auto set_done_components = std::set<size_t>();
	for (auto vert_pair = boost::vertices(prob_graph);
		vert_pair.first != vert_pair.second; ++vert_pair.first) {

		auto component_idx = connected_map[*vert_pair.first];
		if(set_done_components.count(component_idx) == 0) {
			set_done_components.insert(component_idx);
			connected_component_vertices.emplace_back(*vert_pair.first);
		}
		
	}
	
}
