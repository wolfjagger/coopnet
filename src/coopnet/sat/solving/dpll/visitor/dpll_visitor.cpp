#include "dpll_visitor.h"

using namespace coopnet;



namespace {

	DPLLVisitorTuple make_visitor_tuple(
		alphali::collaborator&& vertContradictionCollab,
		alphali::collaborator&& edgeContradictionCollab,
		alphali::collaborator& mainContradictCollab,
		alphali::publisher& mainUncontradictPub,
		PruneInfo& pruneInfo,
		DPLLPropMaps maps) {

		return std::make_pair(
			DPLLVertVisitor(
				std::move(vertContradictionCollab),
				mainContradictCollab, mainUncontradictPub,
				pruneInfo, maps),
			DPLLEdgeVisitor(
				std::move(edgeContradictionCollab),
				mainContradictCollab, mainUncontradictPub,
				pruneInfo, maps));

	}

}



DPLLVisitor::DPLLVisitor(
	alphali::collaborator&& vertContradictionCollab,
	alphali::collaborator&& edgeContradictionCollab,
	alphali::collaborator& mainContradictCollab,
	alphali::publisher& mainUncontradictPub,
	PruneInfo& pruneInfo,
	DPLLPropMaps maps) :
	boost::bfs_visitor<DPLLVisitorTuple>(
		make_visitor_tuple(
			std::move(vertContradictionCollab),
			std::move(edgeContradictionCollab),
			mainContradictCollab, mainUncontradictPub,
			pruneInfo, maps)) {

}
