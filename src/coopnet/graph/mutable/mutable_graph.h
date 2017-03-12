#pragma once

#include "extended_graph.h"



namespace coopnet {

	struct MutableSatVProp : public BaseSatVProp {
		mutable struct Mutable {
			PruneStatus status;
			boost::tribool assignment;
		} mutate;

		// Would like to move into mutate, but not sure how to define
		//  a property_map pointing to this "subproperty" value.
		mutable default_color_type color;

		MutableSatVProp() :
			BaseSatVProp(),
			mutate{
			PruneStatus::Active,
			boost::indeterminate},
			color(default_color_type::black_color) {}

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

	template<typename Graph>
	using SatColorPropMap
		= typename boost::property_map<Graph, default_color_type MutableSatVProp::*>::type;




	using PrunableSatGraph = ExtendedSatGraph<MutableSatVProp, MutableSatEProp>;

}
