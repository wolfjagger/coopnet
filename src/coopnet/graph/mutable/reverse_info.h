#pragma once

#include <iostream>
#include <stack>
#include "boost/variant.hpp"
#include "prune_graph.h"
#include "assignable_graph.h"



namespace coopnet {

	using VertStatusPair = std::pair<VertDescriptor, PruneStatus>;
	using EdgeStatusPair = std::pair<EdgeDescriptor, PruneStatus>;
	using IncompleteAssignment
		= std::unordered_map<VertDescriptor, boost::tribool>;
	using IncompleteAssignmentPair = IncompleteAssignment::value_type;


	struct ReversableAction {

		enum class ReverseCategory { Vertex, Edge, Assignment };

		ReverseCategory type;
		boost::variant<VertStatusPair, EdgeStatusPair,
			IncompleteAssignmentPair> suppData;

		explicit ReversableAction(VertStatusPair pruneData);
		explicit ReversableAction(EdgeStatusPair pruneData);
		explicit ReversableAction(IncompleteAssignmentPair pruneData);

	};

	using ReverseStack = std::stack<ReversableAction>;

}
