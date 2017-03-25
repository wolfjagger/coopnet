#pragma once

#include "coopnet/graph/mutable/mutable_graph.h"
#include "dpll_status.h"



namespace coopnet {

	struct DPLLVProp : public MutableSatVProp {
		mutable struct DPLL {
			DPLLVertStatus status;
		} dpll;

		DPLLVProp() :
			MutableSatVProp(),
			dpll{DPLLVertStatus::Default} { }

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
	
}
