#pragma once

#include "graph/bgl_inc.h"
#include "component/component_fwd.h"



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
	using graph = adjacency_list <
		boost::setS, boost::vecS, boost::undirectedS,
		vert_prop, edge_prop>;
	using vertex_descriptor = graph::vertex_descriptor;
	using edge_descriptor = graph::edge_descriptor;



	// Ease of use for adding node and property to graph
	vertex_descriptor add_vertex(graph& g, node n);
	// Ease of use for adding clause and property to graph
	vertex_descriptor add_vertex(graph& g, const clause& c);
	// Ease of use for adding clause and property to graph
	edge_descriptor add_edge(graph& g,
		vertex_descriptor node_desc,
		vertex_descriptor clause_desc,
		bool sgn);

}
