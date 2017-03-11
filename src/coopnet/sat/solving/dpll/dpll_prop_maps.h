#pragma once

#include "coopnet/graph/mutable_graph.h"
#include "dpll_status.h"



namespace coopnet {

	struct DPLLVProp : public MutableSatVProp {
		mutable struct DPLL {
			DPLLVertStatus status;
		} dpll;
		mutable default_color_type color;

		DPLLVProp() :
			MutableSatVProp(),
			dpll{DPLLVertStatus::Default},
			color(default_color_type::black_color) { }

	};

	struct DPLLEProp : public MutableSatEProp {
		mutable struct DPLL {
			DPLLEdgeStatus status;
		} dpll;

		DPLLEProp() :
			MutableSatEProp(),
			dpll{DPLLEdgeStatus::Default} {}

	};



	using DPLLSatGraph = SatGraph<DPLLVProp, DPLLEProp>;

	using DPLLExtSatGraph = ExtendedSatGraph<DPLLVProp, DPLLEProp>;
	


	using DPLLColorPropMap
		= boost::property_map<DPLLSatGraph, default_color_type DPLLVProp::*>::type;
}
