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

vertex_descriptor sat::add_vertex(graph& g, const clause& c) {

	auto prop = graph::vertex_property_type();
	auto name = std::string("c");
	for (auto lit : c.literals()) {
		name += lit.sgn ? "p" : "n";
		name += std::to_string(lit.n.id);
	}
	prop.name = name;
	prop.kind = vert_prop::Clause;

	return boost::add_vertex(prop, g);

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
