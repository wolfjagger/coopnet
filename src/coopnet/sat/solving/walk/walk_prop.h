#pragma once

#include "coopnet/graph/mutable/assignable_graph.h"
#include "walk_status.h"



namespace coopnet {

	struct WalkVProp : public AssignSatVProp {
		mutable WalkVertStatus walkStatus;

		WalkVProp() :
			AssignSatVProp(),
			walkStatus(WalkVertStatus::Default) { }

	};

	struct WalkEProp : public AssignSatEProp {
		mutable WalkEdgeStatus walkStatus;

		WalkEProp() :
			AssignSatEProp(),
			walkStatus(WalkEdgeStatus::Default) {}

	};



	using WalkSatGraph = SatGraph<WalkVProp, WalkEProp>;

}
