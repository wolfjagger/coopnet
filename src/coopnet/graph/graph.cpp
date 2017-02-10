#include "graph.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"

using namespace sat;



vertex_descriptor sat::add_vertex(graph& g, node n) {
	
	auto prop = graph::vertex_property_type();
	prop.kind = vert_prop::Node;

	auto vert = boost::add_vertex(prop, g);

	set_node_name(g[vert], n);

	return vert;

}

vertex_descriptor sat::add_vertex(graph& g, const clause& c) {

	auto prop = graph::vertex_property_type();
	prop.kind = vert_prop::Clause;

	auto vert = boost::add_vertex(prop, g);

	set_clause_name(g[vert], c);

	return vert;

}



edge_descriptor sat::add_edge(graph& g,
	vertex_descriptor node_desc,
	vertex_descriptor clause_desc,
	bool sgn) {

	auto prop = graph::edge_property_type();
	
	auto desc_pair = add_edge(node_desc, clause_desc, prop, g);
	if (!desc_pair.second) std::exception("Failed to add edge to graph!");

	auto edge = desc_pair.first;
	set_edge_sgn(g[edge], sgn);
	
	return edge;

}



void sat::rename_verts(graph& g, const node_vert_map& node_to_vertex_map) {
	
	auto vert_pair = boost::vertices(g);
	for (auto vert_iter = vert_pair.first;
		vert_iter != vert_pair.second; ++vert_iter) {

		auto vert = *vert_iter;

		switch(g[vert].kind) {
		case vert_prop::Node: {

			auto n = node_to_vertex_map.right.at(vert);
			set_node_name(g[vert], n);

			break;

		}
		case vert_prop::Clause: {

			auto lits = clause::lit_storage();
			auto edge_pair = boost::out_edges(vert, g);
			for (auto edge_iter = edge_pair.first;
				edge_iter != edge_pair.second; ++edge_iter) {

				auto n = node_to_vertex_map.right.at(boost::target(*edge_iter, g));
				lits.emplace(std::make_pair(n, g[*edge_iter].sgn));

			}

			auto c = clause(lits);
			set_clause_name(g[vert], c);

			break;

		}}

	}

}



void sat::set_node_name(
	vert_prop& prop, node n) {

	auto name = std::string("n");
	name += std::to_string(n.id);
	prop.name = name;

}

void sat::set_clause_name(
	vert_prop& prop, const clause& c) {

	auto name = std::string("c");
	for (auto lit : c.literals()) {
		name += lit.second ? "p" : "n";
		name += std::to_string(lit.first.id);
	}
	prop.name = name;

}

void sat::set_edge_sgn(
	edge_prop& prop, bool sgn) {

	prop.sgn = sgn;

}
