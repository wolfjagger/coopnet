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
	reversableGraph(prob.get_graph()),
	greyBuffer(),
	isContradicting(std::make_shared<bool>(false)) {


	colorPropMap = boost::get(&DPLLVProp::color, graph());

	pruneVisitor = std::make_unique<BfsDPLLVisitor>(
		reversableGraph.reverse_stack(),
		isContradicting);

}

DPLLFormula::~DPLLFormula() { }



void DPLLFormula::set_node(DPLLNodeChoice choice) {

	// partial_assign set @ node_to_set=value
	// partial_graph remove node_to_set and reduce (unit clauses
	//  & pure literals), supplying the stack to append

	auto vertNode = node_vert_map().left.at(choice.n);

	auto status = choice.sgn ? 
		DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse;
	graph()[vertNode].dpll.status = status;

	if (DEBUG) std::cout << "Assign node " << choice.n.id <<
		" with vert " << vertNode << " to " << status << std::endl;

	boost::breadth_first_visit(
		graph(), vertNode, greyBuffer,
		*pruneVisitor, colorPropMap);

}



void DPLLFormula::reverse_prune_to_assignment(Node n) {

	auto vert = node_vert_map().left.at(n);
	reversableGraph.reverse_to_vert(vert);

}



auto DPLLFormula::reversable_graph() const
    -> const ReversableGraph&{
    return reversableGraph;
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



IncompleteAssignment DPLLFormula::create_incomplete_assignment() const {

	auto assignment = IncompleteAssignment();
	auto copyPred = [this, &assignment](auto pair) {
		assignment.emplace(pair.second, reversableGraph.get_assignment(pair.second));
	};
	apply_to_node_vert_map(copyPred);

	return assignment;

}

void DPLLFormula::set_incomplete_assignment(const IncompleteAssignment& assignment) {

	auto copyPred = [this, &assignment](auto pair) {
		reversableGraph.set_assignment(pair.second, assignment.at(pair.second));
	};
	apply_to_node_vert_map(copyPred);

}



bool DPLLFormula::is_SAT() const {
	return !reversableGraph.is_indeterminate();
}

Assignment DPLLFormula::create_assignment() const {

	if (reversableGraph.is_indeterminate()) {
		throw std::exception("Incomplete assignment cannot be transformed.");
	}

	Assignment assignment;
	auto copyPred = [this, &assignment](auto pair) {
		assignment.data.emplace(pair.first, bool(reversableGraph.get_assignment(pair.second)));
	};
	apply_to_node_vert_map(copyPred);

	return assignment;

}

void DPLLFormula::set_assignment(const Assignment& assignment) {

	auto copyPred = [this, &assignment](auto pair) {
		reversableGraph.set_assignment(pair.second, boost::tribool(assignment.data.at(pair.first)));
	};
	apply_to_node_vert_map(copyPred);

}




auto DPLLFormula::graph() const -> const Graph& {
	return reversableGraph.get_graph();
}
auto DPLLFormula::graph() -> Graph& {
	return reversableGraph.get_graph();
}
