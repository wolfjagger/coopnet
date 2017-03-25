#pragma once

#include "extended_graph.h"



namespace coopnet {

	struct MutableSatVProp : public BaseSatVProp {
		mutable struct Mutable {
			PruneStatus status;
			boost::tribool assignment;
		} mutate;

		MutableSatVProp() :
			BaseSatVProp(),
			mutate{
			PruneStatus::Active,
			boost::indeterminate} {}

	};

	struct MutableSatEProp : public BaseSatEProp {
		mutable struct Mutable {
			PruneStatus status;
		} mutate;

		MutableSatEProp() :
			BaseSatEProp(),
			mutate{PruneStatus::Active} { }

	};

	using MutableSatGraph = SatGraph<MutableSatVProp, MutableSatEProp>;



	using PrunableSatGraph = ExtendedSatGraph<MutableSatVProp, MutableSatEProp>;

}
