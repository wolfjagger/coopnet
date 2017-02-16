#include "dpll_visitor.h"
#include <iostream>

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}

// This visitor will decide if the vert should be removed:
//  either if it only has one edge (clause)
//  or if it has all edges with same sign (node).
// It also needs to color the surrounding edges if they
//  should be (re)visited (i.e. if vert is to be removed).
void dpll_begin_vert_visitor::node_event(
	const graph& g, vertex_descriptor node, const vert_prop& prop) {

	switch(maps.vert_status_map[node]) {
	case dpll_vert_status::SetToTrue:

		select_node(g, node, true);
		break;

	case dpll_vert_status::SetToFalse:

		select_node(g, node, false);
		break;

	case dpll_vert_status::Active:

		auto edges_pair = boost::out_edges(node, g);

		// If there are no edges, select node freely
		auto inactive_edges_fcn = [this](edge_descriptor e) {
			return maps.edge_status_map[e] == dpll_edge_status::Inactive;
		};

		if (std::all_of(
			edges_pair.first, edges_pair.second, inactive_edges_fcn)) {

			//TODO: Select at random?
			select_node(g, node, true);

		} else {

			auto constrain_node_fcn = [this](edge_descriptor e) {
				return maps.edge_status_map[e] == dpll_edge_status::ConstrainNode;
			};
			auto constraining_edge = std::find_if(
				edges_pair.first, edges_pair.second, constrain_node_fcn);

			// If any edge is set to constrain the node, constrain it
			//  (select_node will check for multiple conflicting constraints)
			if (constraining_edge != edges_pair.second) {
				change_edge_status(*constraining_edge, dpll_edge_status::Inactive);
				select_node(g, node, g[*constraining_edge].sgn);
			}
			// Otherwise, if all active edges have same sgn, select node = sgn
			else {

				auto sgn_first = g[*edges_pair.first].sgn;
				auto all_active_same_sgn_fcn = [sgn_first, &g, this](edge_descriptor e) {
					return (maps.edge_status_map[e] == dpll_edge_status::Active
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

void dpll_begin_vert_visitor::clause_event(
	const graph& g, vertex_descriptor clause, const vert_prop& prop) {

	auto& clause_status = maps.vert_status_map[clause];

	switch(clause_status) {
	case dpll_vert_status::Remove: {

		satisfy_clause(g, clause);
		break;

	}
	case dpll_vert_status::Active: {

		auto edges_pair = boost::out_edges(clause, g);

		auto satisfy_clause_fcn = [this](edge_descriptor e) {
			return maps.edge_status_map[e] == dpll_edge_status::SatisfyClause;
		};
		auto satisfying_edge = std::find_if(
			edges_pair.first, edges_pair.second, satisfy_clause_fcn);

		// If any edge is set to satisfy the clause, satisfy it
		//  (satisfy_clause will check for multiple conflicting constraints)
		if (satisfying_edge != edges_pair.second) {
			change_edge_status(*satisfying_edge, dpll_edge_status::Inactive);
			satisfy_clause(g, clause);
		} else {
			
			auto active_edges_fcn = [this](edge_descriptor e) {
				return maps.edge_status_map[e] == dpll_edge_status::Active;
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

				change_edge_status(*edge, dpll_edge_status::ConstrainNode);
				satisfy_clause(g, clause);

			}
			
		}
		break;

	}}

}



void dpll_begin_vert_visitor::select_node(
	const graph& g, vertex_descriptor node, bool sgn) {

	change_assignment(node, sgn);

	auto edge_pair = boost::out_edges(node, g);
	for (auto edge = edge_pair.first;
		edge != edge_pair.second; ++edge) {

		auto status = maps.edge_status_map[*edge];
		switch (status) {
		case dpll_edge_status::Active:
			// If active edge, use to satisfy clause (if correct sgn) or remove
			if (g[*edge].sgn == sgn) {
				change_edge_status(*edge, dpll_edge_status::SatisfyClause);
			} else {
				change_edge_status(*edge, dpll_edge_status::Remove);
			}
			break;
		case dpll_edge_status::ConstrainNode:
			// If already set to constrain node, set inactive or contradict
			if (g[*edge].sgn == sgn) {
				change_edge_status(*edge, dpll_edge_status::Inactive);
			} else {
				if(DEBUG) std::cout << "Contradict: opposite constraints on node.\n";
				is_contradicting = true;
			}
			break;
		}

	}

	change_vert_status(node, dpll_vert_status::Inactive);

}

void dpll_begin_vert_visitor::satisfy_clause(
	const graph& g, vertex_descriptor clause) {
	
	auto edges_pair = boost::out_edges(clause, g);
	for (auto edge = edges_pair.first;
		edge != edges_pair.second; ++edge) {

		auto status = maps.edge_status_map[*edge];
		switch (status) {
		case dpll_edge_status::Active:
		case dpll_edge_status::SatisfyClause:
			// Even if pushing node info to clause, info is now unnecessary
			change_edge_status(*edge, dpll_edge_status::Remove);
			break;
		}

	}

	change_vert_status(clause, dpll_vert_status::Inactive);

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

void dpll_begin_vert_visitor::change_assignment(
	vertex_descriptor node, boost::logic::tribool new_assign) {
	
	auto& value = maps.partial_assignment_map[node];

	if (DEBUG) std::cout << "node " << node << " assignment " << value << " to " << new_assign << std::endl;
	
	auto prune_data = std::make_pair(node, value);
	prune_action_stack.data.push(prune_action(prune_data));
	value = new_assign;

}

void dpll_begin_vert_visitor::change_vert_status(
	vertex_descriptor vert, dpll_vert_status new_status) {

	auto& status = maps.vert_status_map[vert];
	if (status != new_status) {
		if(DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(vert, status);
		prune_action_stack.data.push(prune_action(prune_data));
		status = new_status;
	}

}

void dpll_begin_vert_visitor::change_edge_status(
	edge_descriptor edge, dpll_edge_status new_status) {

	auto& status = maps.edge_status_map[edge];
	if (status != new_status) {
		if(DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(edge, status);
		prune_action_stack.data.push(prune_action(prune_data));
		status = new_status;
	}

}



// This has three responsibilities.
// (1) color unvisited vertex to visit it
// (2) it needs to propagate removals to the next vertices,
//  with cases depending on the circumstances:
//  (a) node = a => remove clause iff sgn(edge) == a
//  (b) clause => set node = a iff clause.num_edges == 1
// (3) remove edge
void dpll_edge_visitor::edge_event(
	const graph& g, edge_descriptor edge,
	const edge_prop& prop,
	vertex_descriptor node, vertex_descriptor clause) {

	auto status = maps.edge_status_map[edge];

	// If edge is active or inactive, there is no action required;
	//  the edge is not meant to be traversed
	if (status == dpll_edge_status::Active
		|| status == dpll_edge_status::Inactive) return;

	// If edge should be traversed (i.e. has status SatisfyClause, ConstrainNode, or Remove),
	//  propagate white color to target if black
	auto& color = maps.color_map[boost::target(edge, g)];
	if (color == default_color_type::black_color)
		color = default_color_type::white_color;

	switch (status) {
	case dpll_edge_status::SatisfyClause: {

		// If node satisfies clause, remove clause
		auto clause_status = maps.vert_status_map[clause];
		if (clause_status != dpll_vert_status::Inactive) {

			change_vert_status(clause, dpll_vert_status::Remove);

		}
		break;

	}
	case dpll_edge_status::ConstrainNode: {

		// If clause constrains node, set node to sgn
		auto node_status = maps.vert_status_map[node];
		switch (node_status) {
		case dpll_vert_status::Active:

			change_vert_status(node, prop.sgn ?
				dpll_vert_status::SetToTrue : dpll_vert_status::SetToFalse);
			break;

		case dpll_vert_status::SetToTrue:

			if (!prop.sgn) {
				if (DEBUG) std::cout << "Contradict: Can't constrain node to false when already true.\n";
				is_contradicting = true;
			}
			break;

		case dpll_vert_status::SetToFalse:

			if (prop.sgn) {
				if (DEBUG) std::cout << "Contradict: Can't constrain node to true when already false.\n";
				is_contradicting = true;
			}
			break;

		}

		break;

	}}
	
	change_edge_status(edge, dpll_edge_status::Inactive);

}

//TODO: Undo replication here and redundancy btwn the three unique methods
void dpll_edge_visitor::change_vert_status(
	vertex_descriptor vert, dpll_vert_status new_status) {

	auto& status = maps.vert_status_map[vert];
	if (status != new_status) {
		if(DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(vert, status);
		prune_action_stack.data.push(prune_action(prune_data));
		status = new_status;
	}

}

void dpll_edge_visitor::change_edge_status(
	edge_descriptor edge, dpll_edge_status new_status) {

	auto& status = maps.edge_status_map[edge];
	if (status != new_status) {
		if(DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << new_status << std::endl;
		auto prune_data = std::make_pair(edge, status);
		prune_action_stack.data.push(prune_action(prune_data));
		status = new_status;
	}

}



namespace {

	dpll_visitor_tuple make_visitor_tuple(
		prune_stack& prune_action_stack,
		boost::queue<vertex_descriptor>& grey_queue,
		bool& is_contradicting,
		dpll_prop_maps maps) {

		dpll_begin_vert_visitor begin_vert_vis(
			prune_action_stack, is_contradicting, maps);
		dpll_edge_visitor edge_vis(
			prune_action_stack, is_contradicting, maps);
		dpll_finish_vert_visitor finish_vert_vis(
			grey_queue, is_contradicting, maps);
		return std::make_pair(begin_vert_vis,
			std::make_pair(edge_vis, finish_vert_vis));

	}

}

dpll_visitor::dpll_visitor(
	prune_stack& prune_action_stack,
	boost::queue<vertex_descriptor>& grey_queue,
	dpll_prop_maps maps) :
	boost::bfs_visitor<dpll_visitor_tuple>(
		make_visitor_tuple(prune_action_stack, grey_queue, is_contradicting, maps)),
	is_contradicting(false) {

}
