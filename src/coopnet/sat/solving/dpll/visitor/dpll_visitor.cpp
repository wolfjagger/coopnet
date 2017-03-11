#include "dpll_visitor.h"

using namespace coopnet;



namespace {

	DPLLVisitorTuple make_visitor_tuple(
		PruneStack& pruneStack,
		alphali::collaborator&& vertContradictionCollab,
		alphali::collaborator&& edgeContradictionCollab,
		alphali::publisher& mainContradictPub,
		alphali::publisher& mainUncontradictPub) {

		return std::make_pair(
			DPLLVertVisitor(
				pruneStack,
				std::move(vertContradictionCollab),
				mainContradictPub, mainUncontradictPub),
			DPLLEdgeVisitor(
				pruneStack,
				std::move(edgeContradictionCollab),
				mainContradictPub, mainUncontradictPub)
		);

	}

}



DPLLVisitor::DPLLVisitor(
	PruneStack& pruneStack,
	alphali::collaborator&& vertContradictionCollab,
	alphali::collaborator&& edgeContradictionCollab,
	alphali::publisher& mainContradictPub,
	alphali::publisher& mainUncontradictPub) :
	boost::bfs_visitor<DPLLVisitorTuple>(
		make_visitor_tuple(
			pruneStack,
			std::move(vertContradictionCollab),
			std::move(edgeContradictionCollab),
			mainContradictPub, mainUncontradictPub)) {

}
