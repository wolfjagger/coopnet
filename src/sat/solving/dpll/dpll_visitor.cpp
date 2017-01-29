#include "dpll_visitor.h"

using namespace sat;



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

		}
		// Otherwise, if all edges have same sgn, select node = sgn
		else {

			auto sgn_first = g[*edges_pair.first].sgn;
			auto all_same_fcn = [sgn_first, &g](edge_descriptor e) {
				return sgn_first == g[e].sgn;
			};

			if (std::all_of(
				edges_pair.first, edges_pair.second, all_same_fcn)) {

				select_node(g, node, sgn_first);
				
			}

		}

		break;

	}

}

void dpll_begin_vert_visitor::clause_event(
	const graph& g, vertex_descriptor clause, const vert_prop& prop) {

	if(maps.vert_status_map[clause] == dpll_vert_status::Remove) {

		auto edges_pair = boost::out_edges(clause, g);
		for (auto edge = edges_pair.first;
			edge != edges_pair.second; ++edge) {

			auto& status = maps.edge_status_map[*edge];
			switch (status) {
			case dpll_edge_status::PushClauseToNode:

				throw std::exception("This case shouldn't happen");
				break;

			case dpll_edge_status::PushNodeToClause:

				// If pushing node info to clause, info is now unnecessary
				change_edge_status(*edge, dpll_edge_status::Inactive);

			}

		}

	} else {

		auto edges_pair = boost::out_edges(clause, g);

		auto active_edges_fcn = [this](edge_descriptor e) {
			return maps.edge_status_map[e] != dpll_edge_status::Inactive;
		};
		auto num_active_edges = std::count_if(
			edges_pair.first, edges_pair.second, active_edges_fcn);
		
		if (num_active_edges == 0) {
			//TODO: Shouldn't handle backtracking with exceptions
			is_contradicting = true;
		} else if (num_active_edges == 1) {
			
			auto edge = std::find_if(
				edges_pair.first, edges_pair.second, active_edges_fcn);
			
			auto status = maps.edge_status_map[*edge];
			switch (status) {
			case dpll_edge_status::Active:
				change_edge_status(*edge, dpll_edge_status::PushClauseToNode);
				change_vert_status(clause, dpll_vert_status::Inactive);
				break;
			case dpll_edge_status::PushNodeToClause:
				if (g[*edge].sgn == maps.partial_assignment_map[boost::target(*edge, g)]) {
					change_edge_status(*edge, dpll_edge_status::Inactive);
					change_vert_status(clause, dpll_vert_status::Inactive);
				} else {
					is_contradicting = true;
				}
				break;
			}
			
		}
	}

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
			change_edge_status(*edge, dpll_edge_status::PushNodeToClause);
			break;
		case dpll_edge_status::PushClauseToNode:
			if (g[*edge].sgn == sgn) {
				change_edge_status(*edge, dpll_edge_status::Inactive);
			} else {
				is_contradicting = true;
			}
			break;
		}

	}

	change_vert_status(node, dpll_vert_status::Inactive);

}

void dpll_begin_vert_visitor::change_assignment(
	vertex_descriptor node, boost::logic::tribool new_assign) {
	
	auto& value = maps.partial_assignment_map[node];
	auto prune_data = std::make_pair(node, value);
	value = new_assign;

}

void dpll_begin_vert_visitor::change_vert_status(
	vertex_descriptor vert, dpll_vert_status new_status) {

	auto& status = maps.vert_status_map[vert];
	auto prune_data = std::make_pair(vert, status);
	status = new_status;

}

void dpll_begin_vert_visitor::change_edge_status(
	edge_descriptor edge, dpll_edge_status new_status) {

	auto& status = maps.edge_status_map[edge];
	auto prune_data = std::make_pair(edge, status);
	status = new_status;

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

	// If edge is active, propagate white color to target if black
	if (status != dpll_edge_status::Inactive) {
		auto& color = maps.color_map[boost::target(edge, g)];
		if (color == default_color_type::black_color)
			color = default_color_type::white_color;
	}

	switch (status) {
	case dpll_edge_status::PushNodeToClause: {

		// If pushing node to clause, remove clause iff sgn = sgn(node)
		auto clause_status = maps.vert_status_map[clause];
		if (clause_status != dpll_vert_status::Inactive
			&& prop.sgn == maps.partial_assignment_map[node]) {

			change_vert_status(clause, dpll_vert_status::Remove);

		}
		break;

	}
	case dpll_edge_status::PushClauseToNode: {

		// If pushing clause to node, set node to sgn
		auto node_status = maps.vert_status_map[node];
		switch (node_status) {
		case dpll_vert_status::Active:

			change_vert_status(node, prop.sgn ?
				dpll_vert_status::SetToTrue : dpll_vert_status::SetToFalse);
			break;

		case dpll_vert_status::SetToTrue:

			if (!prop.sgn) is_contradicting = true;
			break;

		case dpll_vert_status::SetToFalse:

			if (prop.sgn) is_contradicting = true;
			break;

		}

		break;

	}
	}
	
	change_edge_status(edge, dpll_edge_status::Inactive);

}

//TODO: Undo replication here and redundancy btwn the three unique methods
void dpll_edge_visitor::change_vert_status(
	vertex_descriptor vert, dpll_vert_status new_status) {

	auto& status = maps.vert_status_map[vert];
	auto prune_data = std::make_pair(vert, status);
	status = new_status;

}

void dpll_edge_visitor::change_edge_status(
	edge_descriptor edge, dpll_edge_status new_status) {

	auto& status = maps.edge_status_map[edge];
	auto prune_data = std::make_pair(edge, status);
	status = new_status;

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
