#include "walk_visitor.h"

using namespace coopnet;



namespace {

	WalkVisitorTuple make_visitor_tuple() {

		return std::make_pair(
			WalkVertVisitor(),
			WalkEdgeVisitor()
		);

	}

}



WalkVisitor::WalkVisitor() :
	boost::bfs_visitor<WalkVisitorTuple>(
		make_visitor_tuple()) {

}
