#include "problem.h"
#include <queue>
#include "boost/graph/breadth_first_search.hpp"
#include "boost/graph/connected_components.hpp"
#include "coopnet/sat/visitor/satisfiability_visitor.h"
#include "assignment.h"
#include "literal_shuffler.h"

using namespace sat;



namespace {

	boost::dynamic_properties generate_dyn_props(
		graph& prob_graph) {

		boost::dynamic_properties dyn_props;

		// Generate dynamic_properties
		auto name_map = get(&vert_prop::name, prob_graph);
		dyn_props.property("name", name_map);
		auto kind_map = get(&vert_prop::kind, prob_graph);
		dyn_props.property("kind", kind_map);
		auto sgn_map = get(&edge_prop::sgn, prob_graph);
		dyn_props.property("sign", sgn_map);

		return dyn_props;

	}

	std::vector<vertex_descriptor>
		calculate_connected_components(const graph& prob_graph) {

		using conn_map_type = std::map<vertex_descriptor, size_t>;
		auto connected_map = conn_map_type();
		auto boost_conn_map =
			boost::associative_property_map<conn_map_type>(connected_map);
		auto num_connected_components
			= boost::connected_components(prob_graph, boost_conn_map);

		auto connected_component_vertices = std::vector<vertex_descriptor>();
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



clause_satisfiability problem::clause_satisfiability_for(
	std::shared_ptr<const assignment> assign) const {

	auto satisfiability_collector = collect_satisfiability_visitor(assign);
	auto bfs_visitor = boost::make_bfs_visitor(satisfiability_collector);

	auto sources = std::vector<size_t>();
	for (auto source_vert : connected_component_vertices)
		sources.push_back(boost::vertex(source_vert, prob_graph));

	auto buffer = boost::queue<vertex_descriptor>();

	using vec_color_type = std::vector<vertex_descriptor>;
	auto vec_colors = vec_color_type(boost::num_vertices(prob_graph));
	auto color_map = boost::make_iterator_property_map(
			vec_colors.begin(), get(boost::vertex_index, prob_graph));

	boost::breadth_first_search(
		prob_graph, sources.cbegin(), sources.cend(), buffer,
		bfs_visitor, color_map);
	
	return *satisfiability_collector.satisfiability;
	
}



std::shared_ptr<const assignment> problem::create_same_sgn_assignment(bool sgn) const {

	// Should be satisfied by an assignment of all true
	auto map_assign = std::map<sat::vertex_descriptor, bool>();
	for (auto i = 0; i < num_nodes; ++i) {
		auto vert = map_node_to_vert->at(i);
		map_assign.emplace(vert, sgn);
	}
	auto assign = std::make_shared<sat::assignment>();
	assign->data = std::move(map_assign);
	return assign;
	
}



void problem::apply_shuffle(const literal_shuffler& shuffler) {

	// Copy swap
	auto map_cpy = *map_node_to_vert;
	for(auto i=0; i < shuffler.literals.size(); ++i) {

		auto lit = shuffler.literals[i];
		auto vert = map_node_to_vert->at(lit.n);

		map_cpy.at(i) = vert;

	}
	std::swap(map_cpy, *map_node_to_vert);

	for(auto i=0; i < shuffler.literals.size(); ++i) {

		auto lit = shuffler.literals[i];
		auto vert = map_node_to_vert->at(i);
		change_vertex(prob_graph, vert, i);

		if (!lit.sgn) {

			auto edge_pair = boost::out_edges(vert, prob_graph);
			for_each(edge_pair.first, edge_pair.second, [this](edge_descriptor e) {
				prob_graph[e].sgn = !prob_graph[e].sgn;
			});

		}
	}

}
		



void problem::build_graph(node_list&& nodes, clause_list&& clauses) {

	num_nodes = nodes.size();
	num_clauses = clauses.size();

	prob_graph = graph();

	// Temp map to connect node-clause edges
	map_node_to_vert = std::make_shared<std::map<node, vertex_descriptor>>();

	// Add all nodes in sequence to graph
	for(auto node_to_add : nodes) {

		// Add node as vertex to graph
		auto node_vert = add_vertex(prob_graph, node_to_add);
		map_node_to_vert->emplace(node_to_add, node_vert);

	}


	// Add all clauses in sequence to graph, then add all edges
	for(auto& clause_to_add : clauses) {

		// Add clause as vertex to graph
		auto clause_vert = add_vertex(prob_graph, clause_to_add);
		for(auto& lit : clause_to_add.literals()) {

			// Add node in clause as edge to graph
			//TODO: Add error handling for if node not in map.
			auto node_vert = map_node_to_vert->at(lit.first);
			add_edge(prob_graph, node_vert, clause_vert, lit.second);

		}

	}


	dyn_props = generate_dyn_props(prob_graph);
	connected_component_vertices = calculate_connected_components(prob_graph);
	num_connected_components = connected_component_vertices.size();
	
}
