#include "dpll_vert_visitor.h"
#include <iostream>

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



DPLLVertVisitor::DPLLVertVisitor(
	PruneStack& initPruneStack,
	alphali::collaborator&& initContradictCollab,
	alphali::collaborator& mainContradictCollab,
	alphali::publisher& mainUncontradictPub,
	DPLLPropMaps initMaps) :
	PruningSatVertVisitor<DPLLVertVisitor>(initPruneStack),
	maps(initMaps) {

	set_uncontradicting();

	contradictionCollab = std::move(initContradictCollab);

	contradictionCollab.subscribe(mainContradictCollab,
		[this]() { set_contradicting(); });
	contradictionCollab.subscribe(mainUncontradictPub,
		[this]() { set_uncontradicting(); });

}



// This visitor will decide if the vert should be removed:
//  either if it only has one edge (clause)
//  or if it has all edges with same sign (node).
// It also needs to color the surrounding edges if they
//  should be (re)visited (i.e. if vert is to be removed).
void DPLLVertVisitor::dpll_node_event(
	const MutableSatGraph& g, VertDescriptor node, const MutableSatVProp& prop) {

	switch (maps.vertStatusMap[node]) {
	case DPLLVertStatus::SetToTrue:

		select_node(g, node, prop, true);
		break;

	case DPLLVertStatus::SetToFalse:

		select_node(g, node, prop, false);
		break;

	case DPLLVertStatus::Remove:

		throw std::exception("Node inappropriately set to 'Remove'.");

	case DPLLVertStatus::Default:

		// If there are no active edges, select node freely
		if (!any_active_edge(node, g)) {

			//TODO: Select at random?
			select_node(g, node, prop, true);

		} else {

			auto edges_pair = boost::out_edges(node, g);
			auto constrain_node_fcn = [this](EdgeDescriptor e) {
				return maps.edgeStatusMap[e] == DPLLEdgeStatus::ConstrainNode;
			};

			auto constraining_edge = std::find_if(
				edges_pair.first, edges_pair.second, constrain_node_fcn);

			// If any edge is set to constrain the node, constrain it
			//  (select_node will check for multiple conflicting constraints)
			if (constraining_edge != edges_pair.second) {

				auto& eProp = g[*constraining_edge];
				deactivate_edge(*constraining_edge, eProp);
				select_node(g, node, prop, g[*constraining_edge].base.sgn);

			}
			// Otherwise, if all active edges have same sgn, select node = sgn
			else {

				auto first_active_edge = find_active_edge(node, g);
				auto first_sgn = g[*first_active_edge].base.sgn;
				auto same_sgn_fcn = [first_sgn, &g, this](EdgeDescriptor e) {
					return first_sgn == g[e].base.sgn;
				};

				if (all_of_active_edges(node, g, same_sgn_fcn))
					select_node(g, node, prop, first_sgn);

			}

		}

		break;

	default:
		throw std::exception("Unknown DPLLVertStatus.");
	}

}

void DPLLVertVisitor::dpll_clause_event(
	const MutableSatGraph& g, VertDescriptor clause, const MutableSatVProp& prop) {

	auto& clause_status = maps.vertStatusMap[clause];

	switch (clause_status) {
	case DPLLVertStatus::Remove: {

		satisfy_clause(g, clause, prop);
		break;

	}
	case DPLLVertStatus::SetToTrue:
	case DPLLVertStatus::SetToFalse: {

		auto stream = std::stringstream();
		stream << "Clause inappropriately set to '";
		stream << clause_status << "'.";
		auto str = stream.str();
		throw std::exception(str.c_str());

	}
	case DPLLVertStatus::Default: {

		auto edges_pair = boost::out_edges(clause, g);

		auto satisfy_clause_fcn = [this](EdgeDescriptor e) {
			return maps.edgeStatusMap[e] == DPLLEdgeStatus::SatisfyClause;
		};
		auto satisfying_edge = find_if_active_edge(clause, g, satisfy_clause_fcn);

		// If any edge is set to satisfy the clause, satisfy it
		//  (satisfy_clause will check for multiple conflicting constraints)
		if (satisfying_edge != edges_pair.second) {

			auto& eProp = g[*satisfying_edge];
			deactivate_edge(*satisfying_edge, eProp);
			satisfy_clause(g, clause, prop);

		} else {

			auto num_active_edges = count_active_edges(clause, g);

			// If no edges left, no way to satisfy clause: contradicting
			if (num_active_edges == 0) {
				if (DEBUG) std::cout << "Contradict: no edges to satisfy clause.\n";
				contradictionCollab.publish();
			}
			// If one edge left, it must be used to satisfy clause
			else if (num_active_edges == 1) {

				auto edge = find_active_edge(clause, g);

				change_edge_status(*edge, DPLLEdgeStatus::ConstrainNode);
				satisfy_clause(g, clause, prop);

			}

		}
		break;

	}
	default:
		throw std::exception("Unknown DPLLVertStatus.");
	}

}

void DPLLVertVisitor::default_vert_event(
	const MutableSatGraph& g, VertDescriptor vert, const MutableSatVProp& prop) {

	maps.vertStatusMap[vert] = DPLLVertStatus::Default;

}



void DPLLVertVisitor::select_node(
	const MutableSatGraph& g, VertDescriptor node, const MutableSatVProp& prop, bool sgn) {

	set_assignment(node, prop.mutate.assignment, sgn);

	auto prop_to_edges_fcn = [this, &g, sgn](EdgeDescriptor edge) {

		auto status = maps.edgeStatusMap[edge];
		switch (status) {
		case DPLLEdgeStatus::Default:
			// If active edge, use to satisfy clause (if correct sgn) or remove
			if (g[edge].base.sgn == sgn) {
				change_edge_status(edge, DPLLEdgeStatus::SatisfyClause);
			} else {
				change_edge_status(edge, DPLLEdgeStatus::Remove);
			}
			break;
		case DPLLEdgeStatus::ConstrainNode:
			// If already set to constrain node, set inactive or contradict
			auto& eProp = g[edge];
			if (eProp.base.sgn == sgn) {
				deactivate_edge(edge, eProp);
			} else {
				if (DEBUG) std::cout << "Contradict: opposite constraints on node.\n";
				contradictionCollab.publish();
			}
			break;
		}

	};

	for_each_active_edge(node, g, prop_to_edges_fcn);

	deactivate_vert(node, prop);

}



void DPLLVertVisitor::satisfy_clause(
	const MutableSatGraph& g, VertDescriptor clause, const MutableSatVProp& prop) {

	auto remove_edges_fcn = [this](EdgeDescriptor edge) {

		auto status = maps.edgeStatusMap[edge];
		switch (status) {
		case DPLLEdgeStatus::Default:
		case DPLLEdgeStatus::SatisfyClause:
			// Even if pushing node info to clause, info is now unnecessary
			change_edge_status(edge, DPLLEdgeStatus::Remove);
			break;
		}

	};

	for_each_active_edge(clause, g, remove_edges_fcn);

	deactivate_vert(clause, prop);

}



void DPLLVertVisitor::deactivate_vert(
	VertDescriptor vert, const MutableSatVProp& prop) {
	change_vert_status(vert, DPLLVertStatus::Default);
	set_prune_status(vert, prop.mutate.status, PruneStatus::Inactive);
}

void DPLLVertVisitor::deactivate_edge(
	EdgeDescriptor edge, const MutableSatEProp& prop) {
	change_edge_status(edge, DPLLEdgeStatus::Default);
	set_prune_status(edge, prop.mutate.status, PruneStatus::Inactive);
}




void DPLLVertVisitor::change_vert_status(
	VertDescriptor vert, DPLLVertStatus newStatus) {

	auto& status = maps.vertStatusMap[vert];
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLVertVisitor::change_edge_status(
	EdgeDescriptor edge, DPLLEdgeStatus newStatus) {

	auto& status = maps.edgeStatusMap[edge];
	if (status != newStatus) {
		if (DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}



void DPLLVertVisitor::set_contradicting() {
	isContradicting = true;
}

void DPLLVertVisitor::set_uncontradicting() {
	isContradicting = false;
}
