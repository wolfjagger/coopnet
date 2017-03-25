#pragma once

#include "coopnet/graph/graph.h"
#include "walk_status.h"



namespace coopnet {

	struct WalkVProp : public BaseSatVProp {
		mutable struct Walk {
			WalkVertStatus status;
			bool assignment;
		} walk;

		WalkVProp() :
			BaseSatVProp(),
			walk{ WalkVertStatus::Default } { }

	};

	struct WalkEProp : public BaseSatEProp {
		mutable struct Walk {
			WalkEdgeStatus status;
		} walk;

		WalkEProp() :
			BaseSatEProp(),
			walk{ WalkEdgeStatus::Default } {}

	};



	using WalkSatGraph = SatGraph<WalkVProp, WalkEProp>;

}
