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

	inline void init_extra_vert_props(MutableSatVProp& prop) {
		prop.mutate.status = PruneStatus::Active;
		prop.mutate.assignment = boost::indeterminate;
	}

	inline void init_extra_edge_props(MutableSatEProp& prop) {
		prop.mutate.status = PruneStatus::Active;
	}

}
