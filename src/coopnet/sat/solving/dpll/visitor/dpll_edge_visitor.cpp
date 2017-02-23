#include "dpll_edge_visitor.h"
#include <iostream>

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}





DPLLEdgeVisitor::DPLLEdgeVisitor(
	alphali::collaborator&& initContradictionCollab,
	alphali::collaborator& mainContradictCollab,
	alphali::publisher& mainUncontradictPub,
	PruneInfo& initPruneInfo,
	DPLLPropMaps initMaps) :
	PruneSatEdgeVisitor(initPruneInfo),
	maps(initMaps) {

	set_uncontradicting();

	contradictionCollab = std::move(initContradictionCollab);

	contradictionCollab.subscribe(mainContradictCollab,
		std::bind(&DPLLEdgeVisitor::set_contradicting, this));
	contradictionCollab.subscribe(mainUncontradictPub,
		std::bind(&DPLLEdgeVisitor::set_uncontradicting, this));

}



// This has three responsibilities.
// (1) color unvisited vertex to visit it
// (2) it needs to propagate removals to the next vertices,
//  with cases depending on the circumstances:
//  (a) node = a => remove clause iff sgn(edge) == a
//  (b) clause => set node = a iff clause.num_edges == 1
// (3) remove edge
void DPLLEdgeVisitor::dpll_edge_event(
	const SatGraph& g, EdgeDescriptor edge,
	const EdgeProp& prop,
	VertDescriptor node, VertDescriptor clause) {

	auto status = maps.edgeStatusMap[edge];

	// If edge is in default state, there is no action required;
	//  the edge is not meant to be traversed
	if (status == DPLLEdgeStatus::Default) return;

	// If edge should be traversed (i.e. has status SatisfyClause, ConstrainNode, or Remove),
	//  propagate white color to target if black (grey means in queue already)
	auto& color = maps.colorMap[boost::target(edge, g)];
	if (color == default_color_type::black_color)
		color = default_color_type::white_color;


	switch (status) {
	case DPLLEdgeStatus::SatisfyClause: {

		// If node satisfies clause, remove clause

		if (prune_info().get_vert_status(clause) != PruneStatus::Inactive)
			change_vert_status(clause, DPLLVertStatus::Remove);
		break;

	}
	case DPLLEdgeStatus::ConstrainNode: {

		// If clause constrains node, set node to sgn

		if (prune_info().get_vert_status(node) == PruneStatus::Active) {

			switch (maps.vertStatusMap[node]) {
			case DPLLVertStatus::Default:

				change_vert_status(node, prop.sgn ?
					DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse);
				break;

			case DPLLVertStatus::SetToTrue:

				if (!prop.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to false when already true.\n";
					contradictionCollab.publish();
				}
				break;

			case DPLLVertStatus::SetToFalse:

				if (prop.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to true when already false.\n";
					contradictionCollab.publish();
				}
				break;

			}

		}

		break;

	}
	}

	deactivate_edge(edge);

}

void DPLLEdgeVisitor::default_edge_event(
	const SatGraph& g, EdgeDescriptor edge,
	const EdgeProp& prop,
	VertDescriptor node, VertDescriptor clause) {

	maps.edgeStatusMap[edge] = DPLLEdgeStatus::Default;

}



void DPLLEdgeVisitor::deactivate_edge(EdgeDescriptor edge) {
	change_edge_status(edge, DPLLEdgeStatus::Default);
	prune_info().set_edge_status(edge, PruneStatus::Inactive);
}



//TODO: Undo replication here and redundancy btwn the three unique methods
void DPLLEdgeVisitor::change_vert_status(
	VertDescriptor vert, DPLLVertStatus newStatus) {

	auto& status = maps.vertStatusMap[vert];
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLEdgeVisitor::change_edge_status(
	EdgeDescriptor edge, DPLLEdgeStatus newStatus) {

	auto& status = maps.edgeStatusMap[edge];
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
