#include "dpll_vert_visitor.h"
#include <iostream>

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



DPLLVertVisitor::DPLLVertVisitor(
	ReverseStack& initReverseStack,
	std::shared_ptr<bool> isContradictingPtr) :
	PruningSatVertVisitor<DPLLVertVisitor>(initReverseStack) {

	isContradicting = isContradictingPtr;

}



// This visitor will decide if the vert should be removed:
//  either if it only has one edge (clause)
//  or if it has all edges with same sign (node).
void DPLLVertVisitor::dpll_node_event(
	const DPLLSatGraph& g, VertDescriptor node, const DPLLProp::Node& prop) {

	switch (prop.dpll.status) {
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

			auto constrain_node_fcn = [&g](EdgeDescriptor e) {
				return g[e].dpll.status == DPLLEdgeStatus::ConstrainNode;
			};
			auto constraining_edge = find_if_active_edge(node, g, constrain_node_fcn);

			// If any edge is set to constrain the node, constrain it
			//  (select_node will check for multiple conflicting constraints)
			if (constraining_edge != edges_pair.second) {

				auto& eProp = g[*constraining_edge];
				deactivate_edge(*constraining_edge, eProp);
				select_node(g, node, prop, g[*constraining_edge].sgn);

			}
			// Otherwise, if all active edges have same sgn, select node = sgn
			else {

				auto first_active_edge = find_active_edge(node, g);
				auto first_sgn = g[*first_active_edge].sgn;
				auto same_sgn_fcn = [first_sgn, &g](EdgeDescriptor e) {
					return first_sgn == g[e].sgn;
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
	const DPLLSatGraph& g, VertDescriptor clause, const DPLLProp::Clause& prop) {

	auto& clause_status = prop.dpll.status;

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

		auto satisfy_clause_fcn = [&g](EdgeDescriptor e) {
			return g[e].dpll.status == DPLLEdgeStatus::SatisfyClause;
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
				*isContradicting = true;
			}
			// If one edge left, it must be used to satisfy clause
			else if (num_active_edges == 1) {

				auto edge = *find_active_edge(clause, g);

				change_edge_status(edge, g[edge], DPLLEdgeStatus::ConstrainNode);
				satisfy_clause(g, clause, prop);

			}

		}
		break;

	}
	default:
		throw std::exception("Unknown DPLLVertStatus.");
	}

}

void DPLLVertVisitor::default_node_event(
	const DPLLSatGraph& g, VertDescriptor vert, const DPLLProp::Node& prop) {

	prop.dpll.status = DPLLVertStatus::Default;

}

void DPLLVertVisitor::default_clause_event(
	const DPLLSatGraph& g, VertDescriptor vert, const DPLLProp::Clause& prop) {

	prop.dpll.status = DPLLVertStatus::Default;

}



void DPLLVertVisitor::select_node(
	const DPLLSatGraph& g, VertDescriptor node, const DPLLProp::Node& prop, bool sgn) {

	set_assignment(node, prop.assignment, sgn);

	auto prop_to_edges_fcn = [this, &g, sgn](EdgeDescriptor edge) {

		auto status = g[edge].dpll.status;
		switch (status) {
		case DPLLEdgeStatus::Default:
			// If active edge, use to satisfy clause (if correct sgn) or remove
			if (g[edge].sgn == sgn) {
				change_edge_status(edge, g[edge], DPLLEdgeStatus::SatisfyClause);
			} else {
				change_edge_status(edge, g[edge], DPLLEdgeStatus::Remove);
			}
			break;
		case DPLLEdgeStatus::ConstrainNode:
			// If already set to constrain node, set inactive or contradict
			auto& eProp = g[edge];
			if (eProp.sgn == sgn) {
				deactivate_edge(edge, eProp);
			} else {
				if (DEBUG) std::cout << "Contradict: opposite constraints on node.\n";
				*isContradicting = true;
			}
			break;
		}

	};

	for_each_active_edge(node, g, prop_to_edges_fcn);

	deactivate_node(node, prop);

}



void DPLLVertVisitor::satisfy_clause(
	const DPLLSatGraph& g, VertDescriptor clause, const DPLLProp::Clause& prop) {

	auto remove_edges_fcn = [this, &g](EdgeDescriptor edge) {

		auto status = g[edge].dpll.status;
		switch (status) {
		case DPLLEdgeStatus::Default:
		case DPLLEdgeStatus::SatisfyClause:
			// Even if pushing node info to clause, info is now unnecessary
			change_edge_status(edge, g[edge], DPLLEdgeStatus::Remove);
			break;
		}

	};

	for_each_active_edge(clause, g, remove_edges_fcn);

	deactivate_clause(clause, prop);

}



void DPLLVertVisitor::deactivate_node(
	VertDescriptor vert, const DPLLProp::Node& prop) {
	change_node_status(vert, prop, DPLLVertStatus::Default);
	set_prune_status(vert, prop.pruneStatus, PruneStatus::Inactive);
}

void DPLLVertVisitor::deactivate_clause(
	VertDescriptor vert, const DPLLProp::Clause& prop) {
	change_clause_status(vert, prop, DPLLVertStatus::Default);
	set_prune_status(vert, prop.pruneStatus, PruneStatus::Inactive);
}

void DPLLVertVisitor::deactivate_edge(
	EdgeDescriptor edge, const DPLLProp::Edge& prop) {
	change_edge_status(edge, prop, DPLLEdgeStatus::Default);
	set_prune_status(edge, prop.pruneStatus, PruneStatus::Inactive);
}




void DPLLVertVisitor::change_node_status(
	VertDescriptor vert, const DPLLProp::Node& prop, DPLLVertStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLVertVisitor::change_clause_status(
	VertDescriptor vert, const DPLLProp::Clause& prop, DPLLVertStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLVertVisitor::change_edge_status(
	EdgeDescriptor edge, const DPLLProp::Edge& prop, DPLLEdgeStatus newStatus) {

	auto& status = prop.dpll.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}
