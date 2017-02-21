#pragma once

#include "graph.h"



namespace sat {

	using VertStatusMap
		= std::map<VertDescriptor, bool>;
	using VertStatusPropMap
		= boost::associative_property_map<VertStatusMap>;

	using EdgeStatusMap
		= std::map<EdgeDescriptor, bool>;
	using EdgeStatusPropMap
		= boost::associative_property_map<EdgeStatusMap>;



	struct PrunablePropMaps {

		VertStatusPropMap vertStatus;
		EdgeStatusPropMap edgeStatus;

		PrunablePropMaps() = default;

		PrunablePropMaps(
			VertStatusPropMap initVertStatus,
			EdgeStatusPropMap initEdgeStatus) :
			vertStatus(initVertStatus),
			edgeStatus(initEdgeStatus) {}

	};



	class PrunableSatGraph {

	private:

		SatGraph base;
		VertStatusMap vertStatus;
		EdgeStatusMap edgeStatus;

	public:

		PrunableSatGraph();

		PrunableSatGraph(const SatGraph& original);
		PrunableSatGraph(SatGraph&& original);



		SatGraph& graph() {
			return base;
		}
		const SatGraph& graph() const {
			return base;
		}



		PrunablePropMaps prop_maps() {
			return PrunablePropMaps(vertStatus, edgeStatus);
		}



	private:

		void emplace_all_active();
		
	};

}
