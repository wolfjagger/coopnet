#pragma once

#include "boost/graph/breadth_first_search.hpp"
#include "walk_vert_visitor.h"
#include "walk_edge_visitor.h"



namespace coopnet {

	using WalkVisitorTuple =
		std::pair<WalkVertVisitor, WalkEdgeVisitor>;

	class WalkVisitor :
		public boost::bfs_visitor<WalkVisitorTuple> {

	public:

		WalkVisitor();

	};
	
}
