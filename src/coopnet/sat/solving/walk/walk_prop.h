#pragma once

#include "coopnet/graph/mutable/assignable_graph.h"
#include "walk_status.h"



namespace coopnet {

	struct WalkProp {

		struct Node : public AssignSatProp::Node {
			mutable WalkVertStatus walkStatus;
			int breakCount;

			Node() :
				AssignSatProp::Node(),
				walkStatus(WalkVertStatus::Default) {}

		};

		struct Clause : public AssignSatProp::Clause {
			mutable WalkVertStatus walkStatus;

			Clause() :
				AssignSatProp::Clause(),
				walkStatus(WalkVertStatus::Default) {}

		};

		struct Edge : public AssignSatProp::Edge {};

	};



	using WalkSatGraph = SatGraph<WalkProp>;

}
