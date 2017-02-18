#pragma once

#include "coopnet/graph/graph.h"
#include "dpll_status.h"



namespace sat {

	using incomplete_assignment_prop_map
		= boost::associative_property_map<incomplete_assignment::map>;

	using dpll_vert_status_map
		= std::map<vertex_descriptor, dpll_vert_status>;
	using dpll_vert_status_prop_map
		= boost::associative_property_map<dpll_vert_status_map>;

	using dpll_edge_status_map
		= std::map<edge_descriptor, dpll_edge_status>;
	using dpll_edge_status_prop_map
		= boost::associative_property_map<dpll_edge_status_map>;

	using dpll_color_map
		= std::map<vertex_descriptor, default_color_type>;
	using dpll_color_prop_map
		= boost::associative_property_map<dpll_color_map>;

	struct dpll_prop_maps {

		incomplete_assignment_prop_map partial_assignment_map;
		dpll_vert_status_prop_map vert_status_map;
		dpll_edge_status_prop_map edge_status_map;
		dpll_color_prop_map color_map;

		dpll_prop_maps() = default;

		dpll_prop_maps(
			incomplete_assignment_prop_map init_partial_assignment_map,
			dpll_vert_status_prop_map init_vert_status_map,
			dpll_edge_status_prop_map init_edge_status_map,
			dpll_color_prop_map init_color_map) :
			partial_assignment_map(init_partial_assignment_map),
			vert_status_map(init_vert_status_map),
			edge_status_map(init_edge_status_map),
			color_map(init_color_map) {}

	};

}
