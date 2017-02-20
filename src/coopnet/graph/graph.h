#pragma once

#include "boost/bimap.hpp"
#include "coopnet/sat/component/component_fwd.h"
#include "bgl_inc.h"



namespace sat {

	struct VertProp {

		enum VertKind { Node=0, Clause=1 };

		std::string name;
		// Unfortunately, can't use enum here, so int.
		int kind;

	};

	struct EdgeProp {
		
		bool sgn;

	};

	
	// E.g. vecS means std::vector, giving fast access and slow amendment
	using SatGraph = boost::adjacency_list <
		boost::setS, boost::vecS, boost::undirectedS,
		VertProp, EdgeProp>;
	using VertDescriptor = SatGraph::vertex_descriptor;
	using EdgeDescriptor = SatGraph::edge_descriptor;
	using boost::default_color_type;



	// Ease of use for adding node and property to graph
	VertDescriptor add_vertex(SatGraph& g, Node n);
	// Ease of use for adding clause and property to graph
	VertDescriptor add_vertex(SatGraph& g, const Clause& c);
	// Ease of use for adding clause and property to graph
	EdgeDescriptor add_edge(SatGraph& g,
		VertDescriptor node_desc,
		VertDescriptor clause_desc,
		bool sgn);



	using NodeVertMap = boost::bimap<Node, VertDescriptor>;



	void rename_verts(
		SatGraph& g, const NodeVertMap& node_to_vertex_map);

	void set_node_name(VertProp& prop, Node n);
	void set_clause_name(VertProp& prop, const Clause& c);
	void set_edge_sgn(EdgeProp& prop, bool sgn);


	struct ClauseSatisfiability {
		std::set<VertDescriptor> clauses_satisfied;
	};

}
