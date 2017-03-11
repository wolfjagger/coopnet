#include "dpll_edge_visitor.h"
#include <iostream>

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}





DPLLEdgeVisitor::DPLLEdgeVisitor(
	PruneStack& initPruneStack,
	alphali::collaborator&& initContradictionCollab,
	alphali::publisher& mainContradictPub,
	alphali::publisher& mainUncontradictPub) :
	PruningSatEdgeVisitor<DPLLEdgeVisitor>(initPruneStack) {

	set_uncontradicting();

	contradictionCollab = std::move(initContradictionCollab);

	contradictionCollab.subscribe(mainContradictPub,
		[this]() { set_contradicting(); });
	contradictionCollab.subscribe(mainUncontradictPub,
		[this]() { set_uncontradicting(); });

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
	const DPLLEProp& prop,
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

		if (g[clause].mutate.status != PruneStatus::Inactive)
			change_vert_status(clause, g[clause], DPLLVertStatus::Remove);
		break;

	}
	case DPLLEdgeStatus::ConstrainNode: {

		// If clause constrains node, set node to sgn

		if (g[node].mutate.status == PruneStatus::Active) {

			switch (g[node].dpll.status) {
			case DPLLVertStatus::Default:

				change_vert_status(node, g[node], prop.base.sgn ?
					DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse);
				break;

			case DPLLVertStatus::SetToTrue:

				if (!prop.base.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to false when already true.\n";
					contradictionCollab.publish();
				}
				break;

			case DPLLVertStatus::SetToFalse:

				if (prop.base.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to true when already false.\n";
					contradictionCollab.publish();
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
	const DPLLEProp& prop,
	VertDescriptor node, VertDescriptor clause) {

	prop.dpll.status = DPLLEdgeStatus::Default;

}



void DPLLEdgeVisitor::deactivate_edge(
	EdgeDescriptor edge, const DPLLEProp& prop) {
	change_edge_status(edge, prop, DPLLEdgeStatus::Default);
	set_prune_status(edge, prop.mutate.status, PruneStatus::Inactive);
}



//TODO: Undo replication here and redundancy btwn the three unique methods
void DPLLEdgeVisitor::change_vert_status(
	VertDescriptor vert, const DPLLVProp& prop, DPLLVertStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLEdgeVisitor::change_edge_status(
	EdgeDescriptor edge, const DPLLEProp& prop, DPLLEdgeStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}



void DPLLEdgeVisitor::set_contradicting() {
	isContradicting = true;
}

void DPLLEdgeVisitor::set_uncontradicting() {
	isContradicting = false;
}
