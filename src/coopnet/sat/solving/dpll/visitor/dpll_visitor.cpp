#include "dpll_visitor.h"

using namespace coopnet;



namespace {

	DPLLVisitorTuple make_visitor_tuple(
		PruneStack& pruneStack,
		alphali::collaborator&& vertContradictionCollab,
		alphali::collaborator&& edgeContradictionCollab,
		alphali::publisher& mainContradictPub,
		alphali::publisher& mainUncontradictPub,
		DPLLPropMaps maps) {

		return std::make_pair(
			DPLLVertVisitor(
				pruneStack,
				std::move(vertContradictionCollab),
				mainContradictPub, mainUncontradictPub,
				maps),
			DPLLEdgeVisitor(
				pruneStack,
				std::move(edgeContradictionCollab),
				mainContradictPub, mainUncontradictPub,
				maps));

	}

}



DPLLVisitor::DPLLVisitor(
	PruneStack& pruneStack,
	alphali::collaborator&& vertContradictionCollab,
	alphali::collaborator&& edgeContradictionCollab,
	alphali::publisher& mainContradictPub,
	alphali::publisher& mainUncontradictPub,
	DPLLPropMaps maps) :
	boost::bfs_visitor<DPLLVisitorTuple>(
		make_visitor_tuple(
			pruneStack,
			std::move(vertContradictionCollab),
			std::move(edgeContradictionCollab),
			mainContradictPub, mainUncontradictPub,
			maps)) {

}
