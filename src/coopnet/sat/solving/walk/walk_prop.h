#pragma once

#include "coopnet/graph/mutable/assignable_graph.h"



namespace coopnet {

	struct WalkProp {

		struct Node : public AssignSatProp::Node {
			mutable int breakCount;

			Node() :
				AssignSatProp::Node(),
				breakCount(0) {}

		};

		struct Clause : public AssignSatProp::Clause {
			mutable unsigned int numSat;

			Clause() :
				AssignSatProp::Clause(),
				numSat(0) {}

		};

		struct Edge : public AssignSatProp::Edge {};

	};



	using WalkSatGraph = SatGraph<WalkProp>;

}
