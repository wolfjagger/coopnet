#pragma once

#include "coopnet/graph/mutable/reversable_graph.h"
#include "dpll_status.h"



namespace coopnet {

	struct DPLLProp {

		struct Node : public ReversableSatProp::Node {
			mutable struct DPLL {
				DPLLVertStatus status;
			} dpll;

			Node() :
				ReversableSatProp::Node(),
				dpll{ DPLLVertStatus::Default } {}

		};

		struct Clause : public ReversableSatProp::Clause {
			mutable struct DPLL {
				DPLLVertStatus status;
			} dpll;

			Clause() :
				ReversableSatProp::Clause(),
				dpll{ DPLLVertStatus::Default } {}

		};

		struct Edge : public ReversableSatProp::Edge {
			mutable struct DPLL {
				DPLLEdgeStatus status;
			} dpll;

			Edge() :
				ReversableSatProp::Edge(),
				dpll{ DPLLEdgeStatus::Default } {}

		};

	};



	using DPLLSatGraph = SatGraph<DPLLProp>;

	using DPLLRevSatGraph = ReversableSatGraph<DPLLProp>;
	
}
