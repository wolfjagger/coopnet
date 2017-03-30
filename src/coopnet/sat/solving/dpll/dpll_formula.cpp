#include "dpll_formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"
#include "visitor/dpll_visitor.h"

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



DPLLFormula::DPLLFormula(const Problem& prob) :
	Formula<DPLLVProp, DPLLEProp>(prob),
	greyBuffer(),
	isContradicting(std::make_shared<bool>(false)) {


	pruneVisitor = std::make_unique<BfsDPLLVisitor>(
		reversableGraph.reverse_stack(),
		isContradicting);

}

DPLLFormula::~DPLLFormula() { }



void DPLLFormula::set_node(NodeChoice choice) {

	// partial_assign set @ node_to_set=value
	// partial_graph remove node_to_set and reduce (unit clauses
	//  & pure literals), supplying the stack to append

	auto vert_node = node_vert_map().left.at(choice.n);

	auto status = choice.sgn ? 
		DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse;
	graph()[vert_node].dpll.status = status;

	if (DEBUG) std::cout << "Assign node " << choice.n.id <<
		" with vert " << vert_node << " to " << status << std::endl;

	boost::breadth_first_visit(
		graph(), vert_node, greyBuffer,
		*pruneVisitor, colorPropMap);

}



bool DPLLFormula::is_contradicting() const {
	return *isContradicting;
}

void DPLLFormula::set_contradicting() {
	*isContradicting = true;
}

void DPLLFormula::set_uncontradicting() {
	*isContradicting = false;
}
