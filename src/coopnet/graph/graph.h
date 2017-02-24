#pragma once

#include <unordered_map>
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

	template<typename Value>
	using EdgeDescUnordMap
		= std::unordered_map<EdgeDescriptor, Value, boost::hash<EdgeDescriptor>>;



	using NodeVertMap = boost::bimap<Node, VertDescriptor>;



	struct ClauseSatisfiability {
		std::set<VertDescriptor> clauses_satisfied;
	};

}
