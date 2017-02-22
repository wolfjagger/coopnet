#include "dpll_formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



DPLLFormula::DPLLFormula(const Problem& prob) :
	Formula(prob),
	isContradicting(false),
	greyBuffer(),
	vertStatusMap(),
	edgeStatusMap(),
	colorMap() {

	auto vert_iter_pair = boost::vertices(prob_graph());
	auto edge_iter_pair = boost::edges(prob_graph());

	for (auto vert_iter = vert_iter_pair.first;
		vert_iter != vert_iter_pair.second; ++vert_iter) {

		// Initialize all verts to Default
		vertStatusMap.emplace(*vert_iter, DPLLVertStatus::Default);

		// Initialize all colors to black, since visitors will expand
		//  where they need to and breadth_first_visit will set sources
		//  to gray
		colorMap.emplace(*vert_iter, default_color_type::black_color);

	}

	for (auto edge_iter = edge_iter_pair.first;
		edge_iter != edge_iter_pair.second; ++edge_iter) {

		// Initialize all edges to Default
		edgeStatusMap.emplace(*edge_iter, DPLLEdgeStatus::Default);

	}

	propMaps = DPLLPropMaps(
		vertStatusMap, edgeStatusMap, colorMap);
	pruneVisitor = std::make_unique<DPLLVisitor>(
		pruneGraph.prune_info(), greyBuffer, propMaps);

}



void DPLLFormula::set_node(NodeChoice choice) {

	// partial_assign set @ node_to_set=value
	// partial_graph remove node_to_set and reduce (unit clauses
	//  & pure literals), supplying the stack to append

	auto vert_node = node_vert_map().left.at(choice.n);

	auto status = choice.sgn ? 
		DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse;
	propMaps.vertStatusMap[vert_node] = status;

	if (DEBUG) std::cout << "Assign node " << choice.n.id <<
		" with vert " << vert_node << " to " << status << std::endl;

	boost::breadth_first_visit(
		prob_graph(), vert_node, greyBuffer,
		*pruneVisitor, propMaps.colorMap);

}
