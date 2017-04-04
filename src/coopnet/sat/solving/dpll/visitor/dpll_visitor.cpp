#include "dpll_visitor.h"

using namespace coopnet;



namespace {

	DPLLVisitorTuple make_visitor_tuple(
		ReverseStack& reverseStack,
		std::shared_ptr<bool> isContradictingPtr) {

		return std::make_pair(
			DPLLVertVisitor(reverseStack, isContradictingPtr),
			DPLLEdgeVisitor(reverseStack, isContradictingPtr)
		);

	}

}



BfsDPLLVisitor::BfsDPLLVisitor(
	ReverseStack& reverseStack,
	std::shared_ptr<bool> isContradictingPtr) :
	boost::bfs_visitor<DPLLVisitorTuple>(
		make_visitor_tuple(reverseStack, isContradictingPtr)) {

}
