#include "graph.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "boost/graph/connected_components.hpp"

using namespace sat;



VertDescriptor sat::add_vertex(SatGraph& g, Node n) {
	
	auto prop = SatGraph::vertex_property_type();
	prop.kind = VertProp::Node;

	auto vert = boost::add_vertex(prop, g);

	set_node_name(g[vert], n);

	return vert;

}

VertDescriptor sat::add_vertex(SatGraph& g, const Clause& c) {

	auto prop = SatGraph::vertex_property_type();
	prop.kind = VertProp::Clause;

	auto vert = boost::add_vertex(prop, g);

	set_clause_name(g[vert], c);

	return vert;

}



EdgeDescriptor sat::add_edge(SatGraph& g,
	VertDescriptor node_desc,
	VertDescriptor clause_desc,
	bool sgn) {

	auto prop = SatGraph::edge_property_type();
	
	auto desc_pair = add_edge(node_desc, clause_desc, prop, g);
	if (!desc_pair.second) std::exception("Failed to add edge to graph!");

	auto edge = desc_pair.first;
	set_edge_sgn(g[edge], sgn);
	
	return edge;

}



void sat::rename_verts(SatGraph& g, const NodeVertMap& node_to_vertex_map) {
	
	auto vert_pair = boost::vertices(g);
	for (auto vert_iter = vert_pair.first;
		vert_iter != vert_pair.second; ++vert_iter) {

		auto vert = *vert_iter;

		switch(g[vert].kind) {
		case VertProp::Node: {

			auto n = node_to_vertex_map.right.at(vert);
			set_node_name(g[vert], n);

			break;

		}
		case VertProp::Clause: {

			auto lits = Clause::LitStorage();
			auto edge_pair = boost::out_edges(vert, g);
			for (auto edge_iter = edge_pair.first;
				edge_iter != edge_pair.second; ++edge_iter) {

				auto n = node_to_vertex_map.right.at(
					boost::target(*edge_iter, g));
				lits.emplace(std::make_pair(n, g[*edge_iter].sgn));

			}

			auto c = Clause(lits);
			set_clause_name(g[vert], c);

			break;

		}}

	}

}



void sat::set_node_name(
	VertProp& prop, Node n) {

	auto name = std::string("n");
	name += std::to_string(n.id);
	prop.name = name;

}

void sat::set_clause_name(
	VertProp& prop, const Clause& c) {

	auto name = std::string("c");
	for (auto lit : c.literals()) {
		name += lit.second ? "p" : "n";
		name += std::to_string(lit.first.id);
	}
	prop.name = name;

}

void sat::set_edge_sgn(
	EdgeProp& prop, bool sgn) {

	prop.sgn = sgn;

}



std::vector<VertDescriptor>
sat::calculate_connected_components(const SatGraph& prob_graph) {

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
