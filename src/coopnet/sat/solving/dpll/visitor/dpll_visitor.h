#pragma once

#include "boost/graph/breadth_first_search.hpp"
#include "dpll_vert_visitor.h"
#include "dpll_edge_visitor.h"



namespace coopnet {

	using DPLLVisitorTuple =
		std::pair<DPLLVertVisitor, DPLLEdgeVisitor>;

	class DPLLVisitor :
		public boost::bfs_visitor<DPLLVisitorTuple> {

	public:

		DPLLVisitor(
			PruneStack& pruneStack,
			alphali::collaborator&& vertContradictionCollab,
			alphali::collaborator&& edgeContradictionCollab,
			alphali::collaborator& mainContradictCollab,
			alphali::publisher& mainUncontradictPub, 
			DPLLPropMaps initMaps);

	};
	
}
