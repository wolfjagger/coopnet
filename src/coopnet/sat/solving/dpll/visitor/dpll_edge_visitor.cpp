#include "dpll_edge_visitor.h"
#include <iostream>

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}





DPLLEdgeVisitor::DPLLEdgeVisitor(
	ReverseStack& initReverseStack,
	std::shared_ptr<bool> isContradictingPtr) :
	PruningSatEdgeVisitor<DPLLEdgeVisitor>(initReverseStack) {

	isContradicting = isContradictingPtr;

}



// This has three responsibilities.
// (1) color unvisited vertex to visit it
// (2) it needs to propagate removals to the next vertices,
//  with cases depending on the circumstances:
//  (a) node = a => remove clause iff sgn(edge) == a
//  (b) clause => set node = a iff clause.num_edges == 1
// (3) remove edge
void DPLLEdgeVisitor::dpll_edge_event(
	const DPLLSatGraph& g, EdgeDescriptor edge,
	const EProp<DPLLProp>& prop,
	VertDescriptor node, VertDescriptor clause) {

	auto status = g[edge].dpll.status;

	// If edge is in default state, there is no action required;
	//  the edge is not meant to be traversed
	if (status == DPLLEdgeStatus::Default) return;

	// If edge should be traversed (i.e. has status SatisfyClause, ConstrainNode, or Remove),
	//  propagate white color to target if black (grey means in queue already)
	auto& color = g[boost::target(edge, g)].color;
	if (color == default_color_type::black_color)
		color = default_color_type::white_color;


	switch (status) {
	case DPLLEdgeStatus::SatisfyClause: {

		// If node satisfies clause, remove clause

		auto& vProp = g[clause].clause();
		if (vProp.pruneStatus != PruneStatus::Inactive)
			change_clause_status(clause, vProp, DPLLVertStatus::Remove);
		break;

	}
	case DPLLEdgeStatus::ConstrainNode: {

		// If clause constrains node, set node to sgn

		auto& vProp = g[node].node();
		if (vProp.pruneStatus == PruneStatus::Active) {

			switch (vProp.dpll.status) {
			case DPLLVertStatus::Default:

				change_node_status(node, vProp, prop.sgn ?
					DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse);
				break;

			case DPLLVertStatus::SetToTrue:

				if (!prop.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to false when already true.\n";
					*isContradicting = true;
				}
				break;

			case DPLLVertStatus::SetToFalse:

				if (prop.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to true when already false.\n";
					*isContradicting = true;
				}
				break;

			}

		}

		break;

	}
	}

	deactivate_edge(edge, prop);

}

void DPLLEdgeVisitor::default_edge_event(
	const DPLLSatGraph& g, EdgeDescriptor edge,
	const EProp<DPLLProp>& prop,
	VertDescriptor node, VertDescriptor clause) {

	prop.dpll.status = DPLLEdgeStatus::Default;

}



void DPLLEdgeVisitor::deactivate_edge(
	EdgeDescriptor edge, const EProp<DPLLProp>& prop) {
	change_edge_status(edge, prop, DPLLEdgeStatus::Default);
	set_prune_status(edge, prop.pruneStatus, PruneStatus::Inactive);
}



//TODO: Undo replication here and redundancy btwn the three unique methods
void DPLLEdgeVisitor::change_node_status(
	VertDescriptor vert, const DPLLProp::Node& prop, DPLLVertStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLEdgeVisitor::change_clause_status(
	VertDescriptor vert, const DPLLProp::Clause& prop, DPLLVertStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLEdgeVisitor::change_edge_status(
	EdgeDescriptor edge, const DPLLProp::Edge& prop, DPLLEdgeStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}
