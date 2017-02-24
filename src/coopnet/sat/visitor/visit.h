#pragma once

#include "coopnet/graph/prunable_graph.h"



namespace coopnet {

	template<typename Visitor, typename SourceIter>
	void visit_sat_graph(Visitor& v,
		const BaseSatGraph& g, SourceIter sourceBegin, SourceIter sourceEnd) {

		auto bfv = boost::make_bfs_visitor(v);

		auto buffer = boost::queue<VertDescriptor>();

		using vec_color_type = std::vector<VertDescriptor>;
		auto vec_colors = vec_color_type(boost::num_vertices(g));
		auto color_map = boost::make_iterator_property_map(
			vec_colors.begin(), get(boost::vertex_index, g));

		boost::breadth_first_search(
			g, sourceBegin, sourceEnd, buffer, bfv, color_map);

	}

}
