#include "walk_vert_visitor.h"
#include <iostream>

using namespace coopnet;



namespace {
	constexpr bool DEBUG = false;
}



WalkVertVisitor::WalkVertVisitor() :
	SatVertVisitor<WalkVertVisitor>() {

}



void WalkVertVisitor::node_event(
	const WalkSatGraph& g, VertDescriptor node, const WalkVProp& prop) {

	switch (prop.walk.status) {
	case WalkVertStatus::SetToTrue:

		select_node(g, node, prop, true);
		break;

	case WalkVertStatus::SetToFalse:

		select_node(g, node, prop, false);
		break;

	case WalkVertStatus::Remove:

		throw std::exception("Node inappropriately set to 'Remove'.");

	case WalkVertStatus::Default:

		// If there are no active edges, select node freely
		if (!any_active_edge(node, g)) {

			//TODO: Select at random?
			select_node(g, node, prop, true);

		} else {

			auto edges_pair = boost::out_edges(node, g);

			auto constrain_node_fcn = [&g](EdgeDescriptor e) {
				return g[e].walk.status == WalkEdgeStatus::ConstrainNode;
			};
			auto constraining_edge = find_if_active_edge(node, g, constrain_node_fcn);

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
				auto same_sgn_fcn = [first_sgn, &g](EdgeDescriptor e) {
					return first_sgn == g[e].base.sgn;
				};

				if (all_of_active_edges(node, g, same_sgn_fcn))
					select_node(g, node, prop, first_sgn);

			}

		}

		break;

	default:
		throw std::exception("Unknown WalkVertStatus.");
	}

}

void WalkVertVisitor::clause_event(
	const WalkSatGraph& g, VertDescriptor clause, const WalkVProp& prop) {

	auto& clause_status = prop.walk.status;

	switch (clause_status) {
	case WalkVertStatus::Remove: {

		satisfy_clause(g, clause, prop);
		break;

	}
	case WalkVertStatus::SetToTrue:
	case WalkVertStatus::SetToFalse: {

		auto stream = std::stringstream();
		stream << "Clause inappropriately set to '";
		stream << clause_status << "'.";
		auto str = stream.str();
		throw std::exception(str.c_str());

	}
	case WalkVertStatus::Default: {

		auto edges_pair = boost::out_edges(clause, g);

		auto satisfy_clause_fcn = [&g](EdgeDescriptor e) {
			return g[e].walk.status == WalkEdgeStatus::SatisfyClause;
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

				auto edge = *find_active_edge(clause, g);

				change_edge_status(edge, g[edge], WalkEdgeStatus::ConstrainNode);
				satisfy_clause(g, clause, prop);

			}

		}
		break;

	}
	default:
		throw std::exception("Unknown WalkVertStatus.");
	}

}



void WalkVertVisitor::select_node(
	const WalkSatGraph& g, VertDescriptor node, const WalkVProp& prop, bool sgn) {

	set_assignment(node, prop.mutate.assignment, sgn);

	auto prop_to_edges_fcn = [this, &g, sgn](EdgeDescriptor edge) {

		auto status = g[edge].walk.status;
		switch (status) {
		case WalkEdgeStatus::Default:
			// If active edge, use to satisfy clause (if correct sgn) or remove
			if (g[edge].base.sgn == sgn) {
				change_edge_status(edge, g[edge], WalkEdgeStatus::SatisfyClause);
			} else {
				change_edge_status(edge, g[edge], WalkEdgeStatus::Remove);
			}
			break;
		case WalkEdgeStatus::ConstrainNode:
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



void WalkVertVisitor::satisfy_clause(
	const WalkSatGraph& g, VertDescriptor clause, const WalkVProp& prop) {

	auto remove_edges_fcn = [this, &g](EdgeDescriptor edge) {

		auto status = g[edge].walk.status;
		switch (status) {
		case WalkEdgeStatus::Default:
		case WalkEdgeStatus::SatisfyClause:
			// Even if pushing node info to clause, info is now unnecessary
			change_edge_status(edge, g[edge], WalkEdgeStatus::Remove);
			break;
		}

	};

	for_each_active_edge(clause, g, remove_edges_fcn);

	deactivate_vert(clause, prop);

}




void WalkVertVisitor::change_vert_status(
	VertDescriptor vert, const WalkVProp& prop, WalkVertStatus newStatus) {

	auto& status = prop.walk.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void WalkVertVisitor::change_edge_status(
	EdgeDescriptor edge, const WalkEProp& prop, WalkEdgeStatus newStatus) {

	auto& status = prop.walk.status;
	if (status != newStatus) {
		if (DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}
