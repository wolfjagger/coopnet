#include "dpll_visitor.h"

using namespace sat;

	// This visitor will decide if the vert should be removed:
	//  either if it only has one edge (clause)
	//  or if it has all edges with same sign (node).
	// It also needs to color the surrounding edges if they
	//  should be (re)visited (i.e. if vert is to be removed).
void dpll_begin_vert_visitor::node_event(
	const graph& g, vertex_descriptor node, const vert_prop& prop) {

	if (maps.vert_status_map[node] == dpll_vert_status::SetToTrue) {

	}

	// If SetToTrue(False), set node to true (false) and spread
	// If num(edges !Inactive) == 0, set node at random
	// If all edges have same sgn, set node to sgn and spread
	
}

void dpll_begin_vert_visitor::clause_event(
	const graph& g, vertex_descriptor clause, const vert_prop& prop) {

	// If num(edges !Inactive) == 0, fail visit and return to prune
	// If num(edges !Inactive) == 1, remove clause and set edge to PushClauseToNode

}

void dpll_begin_vert_visitor::remove_vert(
	const graph& g, vertex_descriptor descriptor) {

	maps.vert_status_map[descriptor] = dpll_vert_status::Inactive;

	auto edge_iter_pair = boost::out_edges(descriptor, g);
	for (auto edge_iter = edge_iter_pair.first;
		edge_iter != edge_iter_pair.second; ++edge_iter) {

		

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
	edge_prop& prop,
	vertex_descriptor node, vertex_descriptor clause) {

	// If PushNodeToClause, try to remove clause (?)
	// If PushClauseToNode, try to set node to sgn (check if not already set)
	// If !Inactive, color target for visit
	
}



namespace {

	dpll_visitor_tuple make_visitor_tuple(
		incomplete_assignment& assignment,
		prune_stack& prune_action_stack,
		dpll_prop_maps maps) {

		dpll_begin_vert_visitor begin_vert_vis(
			assignment, prune_action_stack, maps);
		dpll_edge_visitor edge_vis(
			assignment, prune_action_stack, maps);
		return std::make_pair(begin_vert_vis, edge_vis);

	}

}

dpll_visitor::dpll_visitor(
	incomplete_assignment& assignment,
	prune_stack& prune_action_stack,
	dpll_prop_maps maps) :
	boost::bfs_visitor<dpll_visitor_tuple>(
		make_visitor_tuple(assignment, prune_action_stack, maps)) {

}
