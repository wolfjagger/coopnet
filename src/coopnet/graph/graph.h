#pragma once

#include <iostream>
#include "boost/bimap.hpp"
#include "coopnet/sat/component/component_fwd.h"
#include "bgl_inc.h"



namespace coopnet {

	using boost::default_color_type;

	struct BaseSatVProp {
		enum VertKind { Node = 0, Clause = 1 };
		struct Base {
			std::string name;
			// Unfortunately, can't use enum here, so int.
			int kind;
		} base;

		// Color for changing what is visited while visiting
		mutable default_color_type color;

		BaseSatVProp() :
			base{ std::string(), 0 },
			color(default_color_type::black_color) { }

	};

	std::ostream& operator<<(std::ostream& os, const BaseSatVProp::Base& base);
	std::istream& operator>>(std::istream& is, BaseSatVProp::Base& base);

	struct BaseSatEProp {
		struct Base {
			bool sgn;
		} base;

		BaseSatEProp() :
			base{true} { }

	};

	std::ostream& operator<<(std::ostream& os, const BaseSatEProp::Base& base);
	std::istream& operator>>(std::istream& is, BaseSatEProp::Base& base);



	// E.g. vecS means std::vector, giving fast access and slow amendment
	template<typename VProp, typename EProp>
	using SatGraph = boost::adjacency_list <
		boost::setS, boost::vecS, boost::undirectedS,
		VProp, EProp>;



	using BaseSatGraph = SatGraph<BaseSatVProp, BaseSatEProp>;



	using VertDescriptor = BaseSatGraph::vertex_descriptor;
	using EdgeDescriptor = BaseSatGraph::edge_descriptor;

	template<typename Value>
	using EdgeDescUnordMap
		= std::unordered_map<EdgeDescriptor, Value, boost::hash<EdgeDescriptor>>;


	template<typename Graph>
	using SatColorPropMap
		= typename boost::property_map<Graph, default_color_type BaseSatVProp::*>::type;



	using NodeVertMap = boost::bimap<Node, VertDescriptor>;



	struct ClauseSatisfiability {
		std::set<VertDescriptor> clausesSatisfied;
	};

}
