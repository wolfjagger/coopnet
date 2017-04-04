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
	reversableGraph(prob.get_graph(), *prob.get_node_vert_translator()),
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

	auto vertNode = choice.vertNode;

	auto status = choice.sgn ? 
		DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse;
	graph()[vertNode].dpll.status = status;

	if (DEBUG) {
		std::cout << "Assign node associated with vert " << vertNode;
		std::cout << " to " << status << std::endl;
	}

	boost::breadth_first_visit(
		graph(), vertNode, greyBuffer,
		*pruneVisitor, colorPropMap);

}



void DPLLFormula::reverse_prune_to_assignment(VertDescriptor vertNode) {

	reversableGraph.reverse_to_vert(vertNode);

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

	auto verts = boost::vertices(graph());
	for (auto vert = verts.first; vert != verts.second; ++vert) {

		auto& prop = graph()[*vert];
		if (prop.base.kind == BaseSatVProp::Node) {
			assignment.emplace(*vert, reversableGraph.get_assignment(*vert));
		}

	}

	return assignment;

}

void DPLLFormula::set_incomplete_assignment(const IncompleteAssignment& assignment) {

	for (auto iter = assignment.begin(); iter != assignment.end(); ++iter) {
		reversableGraph.set_assignment(iter->first, iter->second);
	}

}



bool DPLLFormula::is_SAT() const {
	return !reversableGraph.is_indeterminate();
}




const SatGraphTranslator& DPLLFormula::get_sat_graph_translator() const {
	return reversableGraph.get_translator();
}



auto DPLLFormula::create_vert_assignment() const -> VertAssignment {

	if (reversableGraph.is_indeterminate()) {
		throw std::exception("Incomplete assignment cannot be transformed.");
	}

	auto assignment = VertAssignment();

	auto verts = boost::vertices(graph());
	for (auto vert = verts.first; vert != verts.second; ++vert) {

		auto& prop = graph()[*vert];
		if (prop.base.kind == BaseSatVProp::Node) {
			assignment.emplace(*vert, bool(reversableGraph.get_assignment(*vert)));
		}

	}

	return assignment;

}

void DPLLFormula::set_vert_assignment(const VertAssignment& assignment) {

	for (auto iter = assignment.begin(); iter != assignment.end(); ++iter) {
		reversableGraph.set_assignment(iter->first, boost::tribool(iter->second));
	}

}



auto DPLLFormula::graph() const -> const Graph&{
	return reversableGraph.get_graph();
}
auto DPLLFormula::graph() -> Graph& {
	return reversableGraph.get_graph();
}
