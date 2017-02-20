#include "dpll_visitor.h"
#include <iostream>

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



DPLLBeginVertVisitor::DPLLBeginVertVisitor(
	PruneStack& prune_action_stack,
	bool& is_contradicting,
	DPLLPropMaps maps) :
	prune_action_stack(prune_action_stack),
	is_contradicting(is_contradicting),
	maps(maps) {}



// This visitor will decide if the vert should be removed:
//  either if it only has one edge (clause)
//  or if it has all edges with same sign (node).
// It also needs to color the surrounding edges if they
//  should be (re)visited (i.e. if vert is to be removed).
void DPLLBeginVertVisitor::node_event(
	const SatGraph& g, VertDescriptor node, const VertProp& prop) {

	switch(maps.vert_status_map[node]) {
	case DPLLVertStatus::SetToTrue:

		select_node(g, node, true);
		break;

	case DPLLVertStatus::SetToFalse:

		select_node(g, node, false);
		break;

	case DPLLVertStatus::Active:

		auto edges_pair = boost::out_edges(node, g);

		// If there are no edges, select node freely
		auto inactive_edges_fcn = [this](EdgeDescriptor e) {
			return maps.edge_status_map[e] == DPLLEdgeStatus::Inactive;
		};

		if (std::all_of(
			edges_pair.first, edges_pair.second, inactive_edges_fcn)) {

			//TODO: Select at random?
			select_node(g, node, true);

		} else {

			auto constrain_node_fcn = [this](EdgeDescriptor e) {
				return maps.edge_status_map[e] == DPLLEdgeStatus::ConstrainNode;
			};
			auto constraining_edge = std::find_if(
				edges_pair.first, edges_pair.second, constrain_node_fcn);

			// If any edge is set to constrain the node, constrain it
			//  (select_node will check for multiple conflicting constraints)
			if (constraining_edge != edges_pair.second) {
				change_edge_status(*constraining_edge, DPLLEdgeStatus::Inactive);
				select_node(g, node, g[*constraining_edge].sgn);
			}
			// Otherwise, if all active edges have same sgn, select node = sgn
			else {

				auto sgn_first = g[*edges_pair.first].sgn;
				auto all_active_same_sgn_fcn = [sgn_first, &g, this](EdgeDescriptor e) {
					return (maps.edge_status_map[e] == DPLLEdgeStatus::Active
						&& sgn_first == g[e].sgn);
				};

				if (std::all_of(
					edges_pair.first, edges_pair.second, all_active_same_sgn_fcn)) {

					select_node(g, node, sgn_first);
				
				}

			}

		}

		break;

	}

}

void DPLLBeginVertVisitor::clause_event(
	const SatGraph& g, VertDescriptor clause, const VertProp& prop) {

	auto& clause_status = maps.vert_status_map[clause];

	switch(clause_status) {
	case DPLLVertStatus::Remove: {

		satisfy_clause(g, clause);
		break;

	}
	case DPLLVertStatus::Active: {

		auto edges_pair = boost::out_edges(clause, g);

		auto satisfy_clause_fcn = [this](EdgeDescriptor e) {
			return maps.edge_status_map[e] == DPLLEdgeStatus::SatisfyClause;
		};
		auto satisfying_edge = std::find_if(
			edges_pair.first, edges_pair.second, satisfy_clause_fcn);

		// If any edge is set to satisfy the clause, satisfy it
		//  (satisfy_clause will check for multiple conflicting constraints)
		if (satisfying_edge != edges_pair.second) {
			change_edge_status(*satisfying_edge, DPLLEdgeStatus::Inactive);
			satisfy_clause(g, clause);
		} else {
			
			auto active_edges_fcn = [this](EdgeDescriptor e) {
				return maps.edge_status_map[e] == DPLLEdgeStatus::Active;
			};
			auto num_active_edges = std::count_if(
				edges_pair.first, edges_pair.second, active_edges_fcn);
			
			// If no edges left, no way to satisfy clause: contradicting
			if (num_active_edges == 0) {
				if(DEBUG) std::cout << "Contradict: no edges to satisfy clause.\n";
				is_contradicting = true;
			}
			// If one edge left, it must be used to satisfy clause
			else if (num_active_edges == 1) {
			
				auto edge = std::find_if(
					edges_pair.first, edges_pair.second, active_edges_fcn);

				change_edge_status(*edge, DPLLEdgeStatus::ConstrainNode);
				satisfy_clause(g, clause);

			}
			
		}
		break;

	}}

}



void DPLLBeginVertVisitor::select_node(
	const SatGraph& g, VertDescriptor node, bool sgn) {

	change_assignment(node, sgn);

	auto edge_pair = boost::out_edges(node, g);
	for (auto edge = edge_pair.first;
		edge != edge_pair.second; ++edge) {

		auto status = maps.edge_status_map[*edge];
		switch (status) {
		case DPLLEdgeStatus::Active:
			// If active edge, use to satisfy clause (if correct sgn) or remove
			if (g[*edge].sgn == sgn) {
				change_edge_status(*edge, DPLLEdgeStatus::SatisfyClause);
			} else {
				change_edge_status(*edge, DPLLEdgeStatus::Remove);
			}
			break;
		case DPLLEdgeStatus::ConstrainNode:
			// If already set to constrain node, set inactive or contradict
			if (g[*edge].sgn == sgn) {
				change_edge_status(*edge, DPLLEdgeStatus::Inactive);
			} else {
				if(DEBUG) std::cout << "Contradict: opposite constraints on node.\n";
				is_contradicting = true;
			}
			break;
		}

	}

	change_vert_status(node, DPLLVertStatus::Inactive);

}

void DPLLBeginVertVisitor::satisfy_clause(
	const SatGraph& g, VertDescriptor clause) {
	
	auto edges_pair = boost::out_edges(clause, g);
	for (auto edge = edges_pair.first;
		edge != edges_pair.second; ++edge) {

		auto status = maps.edge_status_map[*edge];
		switch (status) {
		case DPLLEdgeStatus::Active:
		case DPLLEdgeStatus::SatisfyClause:
			// Even if pushing node info to clause, info is now unnecessary
			change_edge_status(*edge, DPLLEdgeStatus::Remove);
			break;
		}

	}

	change_vert_status(clause, DPLLVertStatus::Inactive);

}



namespace {
	std::ostream& operator<<(std::ostream& os, boost::logic::tribool b) {
		if (b) {
			return os << "T";
		} else if (!b) {
			return os << "F";
		} else {
			return os << "I";
		}
	}
}

void DPLLBeginVertVisitor::change_assignment(
	VertDescriptor node, boost::logic::tribool new_assign) {
	
	auto& value = maps.partial_assignment_map[node];

	if (DEBUG) std::cout << "node " << node << " assignment " << value << " to " << new_assign << std::endl;
	
	auto prune_data = std::make_pair(node, value);
	prune_action_stack.data.push(PruneAction(prune_data));
	value = new_assign;

}

void DPLLBeginVertVisitor::change_vert_status(
	VertDescriptor vert, DPLLVertStatus new_status) {

	auto& status = maps.vert_status_map[vert];
	if (status != new_status) {
		if(DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(vert, status);
		prune_action_stack.data.push(PruneAction(prune_data));
		status = new_status;
	}

}

void DPLLBeginVertVisitor::change_edge_status(
	EdgeDescriptor edge, DPLLEdgeStatus new_status) {

	auto& status = maps.edge_status_map[edge];
	if (status != new_status) {
		if(DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(edge, status);
		prune_action_stack.data.push(PruneAction(prune_data));
		status = new_status;
	}

}





DPLLEdgeVisitor::DPLLEdgeVisitor(
	PruneStack& prune_action_stack,
	bool& is_contradicting,
	DPLLPropMaps maps) :
	prune_action_stack(prune_action_stack),
	is_contradicting(is_contradicting),
	maps(maps) {}



// This has three responsibilities.
// (1) color unvisited vertex to visit it
// (2) it needs to propagate removals to the next vertices,
//  with cases depending on the circumstances:
//  (a) node = a => remove clause iff sgn(edge) == a
//  (b) clause => set node = a iff clause.num_edges == 1
// (3) remove edge
void DPLLEdgeVisitor::edge_event(
	const SatGraph& g, EdgeDescriptor edge,
	const EdgeProp& prop,
	VertDescriptor node, VertDescriptor clause) {

	auto status = maps.edge_status_map[edge];

	// If edge is active or inactive, there is no action required;
	//  the edge is not meant to be traversed
	if (status == DPLLEdgeStatus::Active
		|| status == DPLLEdgeStatus::Inactive) return;

	// If edge should be traversed (i.e. has status SatisfyClause, ConstrainNode, or Remove),
	//  propagate white color to target if black
	auto& color = maps.color_map[boost::target(edge, g)];
	if (color == default_color_type::black_color)
		color = default_color_type::white_color;

	switch (status) {
	case DPLLEdgeStatus::SatisfyClause: {

		// If node satisfies clause, remove clause
		auto clause_status = maps.vert_status_map[clause];
		if (clause_status != DPLLVertStatus::Inactive) {

			change_vert_status(clause, DPLLVertStatus::Remove);

		}
		break;

	}
	case DPLLEdgeStatus::ConstrainNode: {

		// If clause constrains node, set node to sgn
		auto node_status = maps.vert_status_map[node];
		switch (node_status) {
		case DPLLVertStatus::Active:

			change_vert_status(node, prop.sgn ?
				DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse);
			break;

		case DPLLVertStatus::SetToTrue:

			if (!prop.sgn) {
				if (DEBUG) std::cout << "Contradict: Can't constrain node to false when already true.\n";
				is_contradicting = true;
			}
			break;

		case DPLLVertStatus::SetToFalse:

			if (prop.sgn) {
				if (DEBUG) std::cout << "Contradict: Can't constrain node to true when already false.\n";
				is_contradicting = true;
			}
			break;

		}

		break;

	}}
	
	change_edge_status(edge, DPLLEdgeStatus::Inactive);

}

//TODO: Undo replication here and redundancy btwn the three unique methods
void DPLLEdgeVisitor::change_vert_status(
	VertDescriptor vert, DPLLVertStatus new_status) {

	auto& status = maps.vert_status_map[vert];
	if (status != new_status) {
		if(DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(vert, status);
		prune_action_stack.data.push(PruneAction(prune_data));
		status = new_status;
	}

}

void DPLLEdgeVisitor::change_edge_status(
	EdgeDescriptor edge, DPLLEdgeStatus new_status) {

	auto& status = maps.edge_status_map[edge];
	if (status != new_status) {
		if(DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(edge, status);
		prune_action_stack.data.push(PruneAction(prune_data));
		status = new_status;
	}

}





DPLLFinishVertVisitor::DPLLFinishVertVisitor(
	boost::queue<VertDescriptor>& grey_buffer,
	bool& is_contradicting,
	DPLLPropMaps maps) :
	grey_buffer(grey_buffer),
	is_contradicting(is_contradicting),
	maps(maps) {}



namespace {

	DPLLVisitorTuple make_visitor_tuple(
		PruneStack& prune_action_stack,
		boost::queue<VertDescriptor>& grey_queue,
		bool& is_contradicting,
		DPLLPropMaps maps) {

		DPLLBeginVertVisitor begin_vert_vis(
			prune_action_stack, is_contradicting, maps);
		DPLLEdgeVisitor edge_vis(
			prune_action_stack, is_contradicting, maps);
		DPLLFinishVertVisitor finish_vert_vis(
			grey_queue, is_contradicting, maps);
		return std::make_pair(begin_vert_vis,
			std::make_pair(edge_vis, finish_vert_vis));

	}

}

DPLLVisitor::DPLLVisitor(
	PruneStack& prune_action_stack,
	boost::queue<VertDescriptor>& grey_queue,
	DPLLPropMaps maps) :
	boost::bfs_visitor<DPLLVisitorTuple>(
		make_visitor_tuple(prune_action_stack, grey_queue, is_contradicting, maps)),
	is_contradicting(false) {

}
