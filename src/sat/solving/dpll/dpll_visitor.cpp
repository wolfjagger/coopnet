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
				status = dpll_edge_status::Inactive;

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
			throw std::exception("Conflict, need to backtrack!");
		} else if (num_active_edges == 1) {
			
			auto edge = std::find_if(
				edges_pair.first, edges_pair.second, active_edges_fcn);
			
			auto& status = maps.edge_status_map[*edge];
			switch (status) {
			case dpll_edge_status::Active:
				status = dpll_edge_status::PushClauseToNode;
				remove_clause(g, clause);
				break;
			case dpll_edge_status::PushNodeToClause:
				if (g[*edge].sgn == maps.partial_assignment_map[boost::target(*edge, g)]) {
					status = dpll_edge_status::Inactive;
					remove_clause(g, clause);
				} else {
					throw std::exception("Conflict, need to backtrack!");
				}
				break;
			}
			
		}
	}

}

void dpll_begin_vert_visitor::remove_clause(
	const graph& g, vertex_descriptor clause) {

	maps.vert_status_map[clause] = dpll_vert_status::Inactive;

}

void dpll_begin_vert_visitor::select_node(
	const graph& g, vertex_descriptor node, bool sgn) {

	maps.partial_assignment_map[node] = sgn;

	auto edge_pair = boost::out_edges(node, g);
	for (auto edge = edge_pair.first;
		edge != edge_pair.second; ++edge) {

		auto& status = maps.edge_status_map[*edge];
		switch (status) {
		case dpll_edge_status::Active:
			status = dpll_edge_status::PushNodeToClause;
			break;
		case dpll_edge_status::PushClauseToNode:
			if (g[*edge].sgn == sgn) {
				status = dpll_edge_status::Inactive;
			} else {
				throw std::exception("Conflict, need to backtrack!");
			}
			break;
		}

	}

	maps.vert_status_map[node] = dpll_vert_status::Inactive;

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

	auto& status = maps.edge_status_map[edge];

	// If edge is active, propagate white color to target if black
	if (status != dpll_edge_status::Inactive) {
		auto& color = maps.color_map[boost::target(edge, g)];
		if (color == default_color_type::black_color)
			color = default_color_type::white_color;
	}

	switch (maps.edge_status_map[edge]) {
	case dpll_edge_status::PushNodeToClause: {

		// If pushing node to clause, remove clause iff sgn = sgn(node)
		auto& clause_status = maps.vert_status_map[clause];
		if (clause_status != dpll_vert_status::Inactive
			&& prop.sgn == maps.partial_assignment_map[node]) {

			clause_status = dpll_vert_status::Remove;

		}
		break;

	}
	case dpll_edge_status::PushClauseToNode: {

		// If pushing clause to node, set node to sgn
		auto& node_status = maps.vert_status_map[node];
		switch (node_status) {
		case dpll_vert_status::Active:

			node_status = prop.sgn ?
				dpll_vert_status::SetToTrue : dpll_vert_status::SetToFalse;
			break;

		case dpll_vert_status::SetToTrue:

			if (!prop.sgn)
				throw std::exception("Conflict detected; backtrack!");
			break;

		case dpll_vert_status::SetToFalse:

			if (prop.sgn)
				throw std::exception("Conflict detected; backtrack!");
			break;

		}

		break;

	}
	}
	
	status = dpll_edge_status::Inactive;
	

}



namespace {

	dpll_visitor_tuple make_visitor_tuple(
		prune_stack& prune_action_stack,
		dpll_prop_maps maps) {

		dpll_begin_vert_visitor begin_vert_vis(
			prune_action_stack, maps);
		dpll_edge_visitor edge_vis(
			prune_action_stack, maps);
		return std::make_pair(begin_vert_vis, edge_vis);

	}

}

dpll_visitor::dpll_visitor(
	prune_stack& prune_action_stack,
	dpll_prop_maps maps) :
	boost::bfs_visitor<dpll_visitor_tuple>(
		make_visitor_tuple(prune_action_stack, maps)) {

}
