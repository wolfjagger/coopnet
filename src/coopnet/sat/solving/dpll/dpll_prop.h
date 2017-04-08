#pragma once

#include "coopnet/graph/mutable/reversable_graph.h"
#include "dpll_status.h"



namespace coopnet {

	struct DPLLProp {

		struct Node : public ReversableSatProp::Node {
			mutable struct DPLL {
				DPLLNodeStatus status;
			} dpll;

			Node() :
				ReversableSatProp::Node(),
				dpll{ DPLLNodeStatus::Default } {}

		};

		struct Clause : public ReversableSatProp::Clause {
			mutable struct DPLL {
				DPLLClauseStatus status;
			} dpll;

			Clause() :
				ReversableSatProp::Clause(),
				dpll{ DPLLClauseStatus::Default } {}

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
