#include "walk_visitor.h"

using namespace coopnet;



WalkVisitor::WalkVisitor(std::shared_ptr<size_t> pNumClausesFailed) :
	numClausesFailed(pNumClausesFailed) {

}



void WalkVisitor::node_event(
	const WalkSatGraph& g, VertDescriptor node,
	const WalkVProp& prop) {

}



void WalkVisitor::clause_event(
	const WalkSatGraph& g, VertDescriptor clause,
	const WalkVProp& prop) {

}
