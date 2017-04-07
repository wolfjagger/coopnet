#pragma once

#include <iostream>
#include "boost/bimap.hpp"
#include "coopnet/sat/component/component_fwd.h"
#include "prop.h"



namespace coopnet {

	// E.g. vecS means std::vector, giving fast access and slow amendment
	template<typename SatProp>
	using SatGraph = boost::adjacency_list <
		boost::setS, boost::vecS, boost::undirectedS,
		typename VProp<SatProp>, typename EProp<SatProp>>;



	using BaseSatGraph = SatGraph<BaseSatProp>;



	using VertDescriptor = BaseSatGraph::vertex_descriptor;
	using EdgeDescriptor = BaseSatGraph::edge_descriptor;

	template<typename Value>
	using EdgeDescUnordMap
		= std::unordered_map<EdgeDescriptor, Value, boost::hash<EdgeDescriptor>>;


	template<typename SatProp>
	using SatColorPropMap
		= typename boost::property_map<SatGraph<SatProp>,
		default_color_type VProp<SatProp>::*>::type;



	using NodeVertMap = boost::bimap<Node, VertDescriptor>;



	struct ClauseSatisfiability {
		std::set<VertDescriptor> clausesSatisfied;
	};

}
