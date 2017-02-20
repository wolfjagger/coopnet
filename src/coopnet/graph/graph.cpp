#include "graph.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"

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
