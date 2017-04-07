#pragma once

#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/graph/base/graph.h"



namespace coopnet {

	template<typename SatGraph, typename Visitor, typename SourceIter>
	void visit_sat_graph(const SatGraph& g, Visitor& v,
		SourceIter sourceBegin, SourceIter sourceEnd) {

		constexpr bool DEBUG = false;

		if (DEBUG) {
			std::cout << "Begin visit_sat_graph\n";
			std::cout << "Construct bfs visitor\n";
		}

		auto bfv = boost::make_bfs_visitor(v);

		auto buffer = boost::queue<VertDescriptor>();

		using vec_color_type = std::vector<VertDescriptor>;
		auto vec_colors = vec_color_type(boost::num_vertices(g));

		if (DEBUG) std::cout << "Construct color map\n";
		auto color_map = boost::make_iterator_property_map(
			vec_colors.begin(), get(boost::vertex_index, g));

		if (DEBUG) std::cout << "Call for boost breadth-first search\n";

		boost::breadth_first_search(
			g, sourceBegin, sourceEnd, buffer, bfv, color_map);

		if (DEBUG) std::cout << "End visit_sat_graph\n";

	}

}
