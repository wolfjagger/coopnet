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
	using IncompleteAssignment
		= std::unordered_map<VertDescriptor, boost::tribool>;
	using IncompleteAssignmentPair = IncompleteAssignment::value_type;


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



	std::ostream& operator<<(std::ostream& os, PruneStatus status);

	std::ostream& operator<<(std::ostream& os, boost::logic::tribool b);

}
