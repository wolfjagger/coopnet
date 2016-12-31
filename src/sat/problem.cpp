#include "problem.h"

using namespace sat;



void problem::add_clause(clause_data init) {
	clauses.emplace(std::move(init));
}

graph problem::build_graph() {

	map_nodes = node_map();
	map_clauses = clause_map();

	auto g = graph();
	for(auto& node_to_add : nodes) {

		auto node_vert = g.add_vertex();
		map_nodes.emplace(node_to_add, node_vert);
		
		std::string name = "Node ";
		name += std::to_string(node_to_add.id);
		put(vertex_color_t::vertex_color, g, node_vert, name);

	}

	for(auto& clause_to_add : clauses) {

		auto clause_vert = g.add_vertex();
		map_clauses.emplace(clause_to_add, clause_vert);
		
		std::string name = "Clause ";
		for (auto node : clause_to_add.nodes()) {
			name += std::to_string(node.id);
		}
		put(vertex_color_t::vertex_color, g, clause_vert, name);

		//for (auto node : clause_to_add.nodes()) {
		for(auto i=0; i<clause_to_add.size(); ++i) {

			//TODO: Add error handling for if node not in map.
			auto node = clause_to_add.nodes()[i];
			auto sgn = clause_to_add.sgns()[i];
			
			auto node_vert = map_nodes.at(node);
			auto edge = g.add_edge(node_vert, clause_vert);
			auto edge_desc = edge.first;
			put(edge_weight_t::edge_weight, g, edge_desc, sgn);

		}

	}
	
	return g;

}



const node_list& problem::list_nodes() const {
	return nodes;
}

const clause_list& problem::list_clauses() const {
	return clauses;
}
