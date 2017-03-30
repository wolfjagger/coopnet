#pragma once

#include "coopnet/graph/mutable/reversable_graph.h"
#include "dpll_status.h"



namespace coopnet {

	struct DPLLVProp : public ReversableSatVProp {
		mutable struct DPLL {
			DPLLVertStatus status;
		} dpll;

		DPLLVProp() :
			ReversableSatVProp(),
			dpll{DPLLVertStatus::Default} { }

	};

	struct DPLLEProp : public ReversableSatEProp {
		mutable struct DPLL {
			DPLLEdgeStatus status;
		} dpll;

		DPLLEProp() :
			ReversableSatEProp(),
			dpll{DPLLEdgeStatus::Default} {}

	};



	using DPLLSatGraph = SatGraph<DPLLVProp, DPLLEProp>;

	using DPLLRevSatGraph = ReversableSatGraph<DPLLVProp, DPLLEProp>;
	
}
