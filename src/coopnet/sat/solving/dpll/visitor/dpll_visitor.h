#pragma once

#include "boost/graph/breadth_first_search.hpp"
#include "dpll_vert_visitor.h"
#include "dpll_edge_visitor.h"



namespace coopnet {

	using DPLLVisitorTuple =
		std::pair<DPLLVertVisitor, DPLLEdgeVisitor>;

	class BfsDPLLVisitor :
		public boost::bfs_visitor<DPLLVisitorTuple> {

	public:

		BfsDPLLVisitor(
			PruneStack& pruneStack,
			alphali::collaborator&& vertContradictionCollab,
			alphali::collaborator&& edgeContradictionCollab,
			alphali::publisher& mainContradictPub,
			alphali::publisher& mainUncontradictPub);

	};
	
}
