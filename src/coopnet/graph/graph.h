#pragma once

#include "boost/bimap.hpp"
#include "coopnet/sat/component/component_fwd.h"
#include "bgl_inc.h"



namespace sat {

	struct vert_prop {

		enum vert_kind { Node=0, Clause=1 };

		std::string name;
		// Unfortunately, can't use enum here, so int.
		int kind;

	};

	struct edge_prop {
		
		bool sgn;

	};

	
	// E.g. vecS means std::vector, giving fast access and slow amendment
	using graph = boost::adjacency_list <
		boost::setS, boost::vecS, boost::undirectedS,
		vert_prop, edge_prop>;
	using vertex_descriptor = graph::vertex_descriptor;
	using edge_descriptor = graph::edge_descriptor;
	using boost::default_color_type;



	// Ease of use for adding node and property to graph
	vertex_descriptor add_vertex(graph& g, node n);
	// Ease of use for adding clause and property to graph
	vertex_descriptor add_vertex(graph& g, const clause& c);
	// Ease of use for adding clause and property to graph
	edge_descriptor add_edge(graph& g,
		vertex_descriptor node_desc,
		vertex_descriptor clause_desc,
		bool sgn);



	using node_vert_map = boost::bimap<node, vertex_descriptor>;



	void rename_verts(
		graph& g, const node_vert_map& node_to_vertex_map);

	void set_node_name(vert_prop& prop, node n);
	void set_clause_name(vert_prop& prop, const clause& c);
	void set_edge_sgn(edge_prop& prop, bool sgn);


	struct clause_satisfiability {
		std::set<vertex_descriptor> clauses_satisfied;
	};

}
