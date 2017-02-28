#pragma once

#include "extended_graph.h"



namespace coopnet {

	struct MutableSatVProp : public BaseSatVProp {
		mutable struct Mutable {
			PruneStatus status;
			boost::tribool assignment;
		} mutate;
	};

	struct MutableSatEProp : public BaseSatEProp {
		mutable struct Mutable {
			PruneStatus status;
		} mutate;
	};

	using MutableSatGraph = SatGraph<MutableSatVProp, MutableSatEProp>;




	using PrunableSatGraph = ExtendedSatGraph<MutableSatVProp, MutableSatEProp>;

}
