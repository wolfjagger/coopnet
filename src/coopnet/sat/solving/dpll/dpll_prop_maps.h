#pragma once

#include "coopnet/graph/graph.h"
#include "dpll_status.h"



namespace sat {

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

		DPLLVertStatusPropMap vertStatusMap;
		DPLLEdgeStatusPropMap edgeStatusMap;
		DPLLColorPropMap colorMap;

		DPLLPropMaps() = default;

		DPLLPropMaps(
			DPLLVertStatusPropMap initVertStatusMap,
			DPLLEdgeStatusPropMap initEdgeStatusMap,
			DPLLColorPropMap initColorMap) :
			vertStatusMap(initVertStatusMap),
			edgeStatusMap(initEdgeStatusMap),
			colorMap(initColorMap) {}

	};

}
