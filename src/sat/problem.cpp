#include "problem.h"

using namespace sat;



const graph& problem::get_graph() const {
	return prob_graph;
}

const dynamic_properties& problem::get_props() const {
	return dyn_props;
}



void problem::build_graph(node_list&& nodes, clause_list&& clauses) {

	prob_graph = graph();

	using node_map = std::map<node, vertex_descriptor>;
	auto map_nodes = node_map();


	// Add all nodes in sequence to graph
	for(auto node_to_add : nodes) {

		// Add node as vertex to graph
		auto node_vert = add_vertex(prob_graph, node_to_add);

		map_nodes.emplace(node_to_add, node_vert);

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
			auto node_vert = map_nodes.at(node);
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
	
}
