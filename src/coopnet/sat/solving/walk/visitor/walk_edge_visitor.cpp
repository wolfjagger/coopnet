#include "walk_edge_visitor.h"
#include <iostream>

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}





WalkEdgeVisitor::WalkEdgeVisitor() :
	SatEdgeVisitor<WalkEdgeVisitor>() {

}



void WalkEdgeVisitor::edge_event(
	const WalkSatGraph& g, EdgeDescriptor edge,
	const WalkEProp& prop,
	VertDescriptor node, VertDescriptor clause) {

	auto status = g[edge].walk.status;

	// If edge is in default state, there is no action required;
	//  the edge is not meant to be traversed
	if (status == WalkEdgeStatus::Default) return;

	// If edge should be traversed (i.e. has status SatisfyClause, ConstrainNode, or Remove),
	//  propagate white color to target if black (grey means in queue already)
	auto& color = g[boost::target(edge, g)].color;
	if (color == default_color_type::black_color)
		color = default_color_type::white_color;


	switch (status) {
	case WalkEdgeStatus::SatisfyClause: {

		// If node satisfies clause, remove clause

		if (g[clause].mutate.status != PruneStatus::Inactive)
			change_vert_status(clause, g[clause], WalkVertStatus::Remove);
		break;

	}
	case WalkEdgeStatus::ConstrainNode: {

		// If clause constrains node, set node to sgn

		if (g[node].mutate.status == PruneStatus::Active) {

			switch (g[node].walk.status) {
			case WalkVertStatus::Default:

				change_vert_status(node, g[node], prop.base.sgn ?
					WalkVertStatus::SetToTrue : WalkVertStatus::SetToFalse);
				break;

			case WalkVertStatus::SetToTrue:

				if (!prop.base.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to false when already true.\n";
					contradictionCollab.publish();
				}
				break;

			case WalkVertStatus::SetToFalse:

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



//TODO: Undo replication here and redundancy btwn the three unique methods
void WalkEdgeVisitor::change_vert_status(
	VertDescriptor vert, const WalkVProp& prop, WalkVertStatus newStatus) {

	auto& status = prop.walk.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void WalkEdgeVisitor::change_edge_status(
	EdgeDescriptor edge, const WalkEProp& prop, WalkEdgeStatus newStatus) {

	auto& status = prop.walk.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}
