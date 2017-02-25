#pragma once

#include <iostream>
#include <stack>
#include "boost/logic/tribool.hpp"
#include "boost/variant.hpp"
#include "graph.h"



namespace coopnet {

	enum class PruneStatus {
		Active, Inactive
	};



	using VertStatusPair = std::pair<VertDescriptor, PruneStatus>;
	using EdgeStatusPair = std::pair<EdgeDescriptor, PruneStatus>;
	using IncompleteAssignmentPair
		= std::pair<VertDescriptor, boost::tribool>;


	struct PruneAction {

		enum class PruneObject { Vertex, Edge, Assignment };

		PruneObject type;
		boost::variant<VertStatusPair, EdgeStatusPair,
			IncompleteAssignmentPair> suppData;

		explicit PruneAction(VertStatusPair pruneData);
		explicit PruneAction(EdgeStatusPair pruneData);
		explicit PruneAction(IncompleteAssignmentPair pruneData);

	};

	using PruneStack = std::stack<PruneAction>;



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



	inline std::ostream& operator<<(std::ostream& os, PruneStatus status);

	inline std::ostream& operator<<(std::ostream& os, boost::logic::tribool b);

}
