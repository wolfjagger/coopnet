#pragma once

#include "coopnet/graph/graph_util.h"
#include "prune_info.h"



namespace coopnet {

	struct PruneSatVProp : public BaseSatVProp {
		mutable struct Mutable {
			PruneStatus status;
			boost::tribool assignment;
		} mutate;

		PruneSatVProp() :
			BaseSatVProp(),
			mutate{
			PruneStatus::Active,
			boost::indeterminate} {}

	};

	struct PruneSatEProp : public BaseSatEProp {
		mutable struct Mutable {
			PruneStatus status;
		} mutate;

		PruneSatEProp() :
			BaseSatEProp(),
			mutate{PruneStatus::Active} { }

	};

	using PruneSatGraph = SatGraph<PruneSatVProp, PruneSatEProp>;

}
