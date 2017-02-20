#pragma once

#include "coopnet/graph/graph.h"
#include "dpll_status.h"



namespace sat {

	using IncompleteAssignmentPropMap
		= boost::associative_property_map<IncompleteAssignment::Map>;

	using DPLLVertStatusMap
		= std::map<VertDescriptor, DPLLVertStatus>;
	using DPLLVertStatusPropMap
		= boost::associative_property_map<DPLLVertStatusMap>;

	using DPLLEdgeStatusMap
		= std::map<EdgeDescriptor, DPLLEdgeStatus>;
	using DPLLEdgeStatusPropMap
		= boost::associative_property_map<DPLLEdgeStatusMap>;

	using DPLLColorMap
		= std::map<VertDescriptor, default_color_type>;
	using DPLLColorPropMap
		= boost::associative_property_map<DPLLColorMap>;

	struct DPLLPropMaps {

		IncompleteAssignmentPropMap partial_assignment_map;
		DPLLVertStatusPropMap vert_status_map;
		DPLLEdgeStatusPropMap edge_status_map;
		DPLLColorPropMap color_map;

		DPLLPropMaps() = default;

		DPLLPropMaps(
			IncompleteAssignmentPropMap init_partial_assignment_map,
			DPLLVertStatusPropMap init_vert_status_map,
			DPLLEdgeStatusPropMap init_edge_status_map,
			DPLLColorPropMap init_color_map) :
			partial_assignment_map(init_partial_assignment_map),
			vert_status_map(init_vert_status_map),
			edge_status_map(init_edge_status_map),
			color_map(init_color_map) {}

	};

}
