#include "graph.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"

using namespace sat;



vertex_descriptor sat::add_vertex(graph& g, node n) {
	
	auto prop = graph::vertex_property_type();

	auto name = std::string("n");
	name += std::to_string(n.id);
	prop.name = name;
	
	prop.kind = vert_prop::Node;

	return boost::add_vertex(prop, g);

}

vertex_descriptor sat::change_vertex(
	graph& g, vertex_descriptor vert, node n) {

	auto name = std::string("n");
	name += std::to_string(n.id);
	g[vert].name = name;

	return vert;

}

vertex_descriptor sat::add_vertex(graph& g, const clause& c) {

	auto prop = graph::vertex_property_type();
	auto name = std::string("c");
	for (auto lit : c.literals()) {
		name += lit.second ? "p" : "n";
		name += std::to_string(lit.first.id);
	}
	prop.name = name;
	prop.kind = vert_prop::Clause;

	return boost::add_vertex(prop, g);

}

vertex_descriptor sat::change_vertex(
	graph& g, vertex_descriptor vert, const clause& c) {

	auto name = std::string("c");
	for (auto lit : c.literals()) {
		name += lit.second ? "p" : "n";
		name += std::to_string(lit.first.id);
	}
	g[vert].name = name;

	return vert;

}



edge_descriptor sat::add_edge(graph& g,
	vertex_descriptor node_desc,
	vertex_descriptor clause_desc,
	bool sgn) {

	auto prop = graph::edge_property_type();
	prop.sgn = sgn;
	
	auto desc_pair = add_edge(node_desc, clause_desc, prop, g);
	if (!desc_pair.second) std::exception("Failed to add edge to graph!");
	
	return desc_pair.first;

}

edge_descriptor sat::change_edge(graph& g,
	vertex_descriptor node_desc,
	vertex_descriptor clause_desc,
	bool sgn) {

	auto desc_pair = boost::edge(node_desc, clause_desc, g);
	g[desc_pair.first].sgn = sgn;
	return desc_pair.first;

}
