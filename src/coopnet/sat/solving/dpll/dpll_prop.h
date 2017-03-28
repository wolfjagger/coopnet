#pragma once

#include "coopnet/graph/mutable/reversable_graph.h"
#include "dpll_status.h"



namespace coopnet {

	struct DPLLVProp : public PruneSatVProp {
		mutable struct DPLL {
			DPLLVertStatus status;
		} dpll;

		DPLLVProp() :
			PruneSatVProp(),
			dpll{DPLLVertStatus::Default} { }

	};

	struct DPLLEProp : public PruneSatEProp {
		mutable struct DPLL {
			DPLLEdgeStatus status;
		} dpll;

		DPLLEProp() :
			PruneSatEProp(),
			dpll{DPLLEdgeStatus::Default} {}

	};



	using DPLLSatGraph = SatGraph<DPLLVProp, DPLLEProp>;

	using DPLLRevSatGraph = ReversableSatGraph<DPLLVProp, DPLLEProp>;
	
}
