#include "walk_visitor.h"

using namespace coopnet;



WalkVisitor::WalkVisitor(std::shared_ptr<unsigned int> pNumClausesFailed) :
	numClausesFailed(pNumClausesFailed) {

	init_count_clauses_failed();

}



void WalkVisitor::node_event(
	const WalkSatGraph& g, VertDescriptor node,
	const WalkVProp& prop) {

}



void WalkVisitor::clause_event(
	const WalkSatGraph& g, VertDescriptor clause,
	const WalkVProp& prop) {

}



void WalkVisitor::init_count_clauses_failed() {

}
