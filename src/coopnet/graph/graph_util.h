#pragma once

#include "graph.h"



namespace sat { namespace graph_util {

	void rename_verts(
		SatGraph& g, const NodeVertMap& node_to_vertex_map);

	std::string node_name(Node n);
	std::string clause_name(const Clause& c);


	std::vector<VertDescriptor>
		calculate_connected_components(const SatGraph& prob_graph);

}}
