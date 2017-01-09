#include "graph.h"
#include "component/node.h"
#include "component/clause.h"

using namespace sat;



vertex_descriptor sat::add_vertex(graph& g, node n) {
	
	auto desc = add_vertex(g);

	auto name = std::string("n");
	name += std::to_string(n.id);
	g[desc].name = name;
	
	g[desc].kind = vert_prop::Node;

	return desc;

}

vertex_descriptor sat::add_vertex(graph& g, const clause& c) {
	
	auto desc = add_vertex(g);

	auto name = std::string("c");
	for (auto n : c.nodes()) name += std::to_string(n.id);
	g[desc].name = name;
	
	g[desc].kind = vert_prop::Clause;

	return desc;

}

edge_descriptor sat::add_edge(graph& g,
	vertex_descriptor node_desc,
	vertex_descriptor clause_desc,
	bool sgn) {
	
	auto desc_pair = add_edge(node_desc, clause_desc, g);

	if (!desc_pair.second) std::exception("Failed to add edge to graph!");
	
	auto desc = desc_pair.first;

	g[desc].sgn = sgn;

	return desc;

}
