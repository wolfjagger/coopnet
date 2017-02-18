#include "dpll_choose_node_visitor.h"

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



dpll_choose_node_visitor::dpll_choose_node_visitor(dpll_prop_maps init_maps) :
	maps(init_maps) {

	reset();

}



void dpll_choose_node_visitor::node_event(
	const graph& g, vertex_descriptor node, const vert_prop& prop) {

	if(maps.vert_status_map[node] == dpll_vert_status::Active) {

		unsigned int num_pos_edges = 0;
		unsigned int num_neg_edges = 0;

		auto edge_pair = boost::out_edges(node, g);
		for (auto edge_iter = edge_pair.first;
			edge_iter != edge_pair.second; ++edge_iter) {

			if (maps.edge_status_map[*edge_iter] == dpll_edge_status::Active) {

				if (g[*edge_iter].sgn) {
					++num_pos_edges;
				} else {
					++num_neg_edges;
				}

			}

		}

		if (num_pos_edges >= num_neg_edges) {
			if(num_pos_edges > max_num_clauses_sat) {
				max_num_clauses_sat = num_pos_edges;
				sgn_sat = true;
				chosen_node = node;
			}
		} else {
			if (num_pos_edges > max_num_clauses_sat) {
				max_num_clauses_sat = num_neg_edges;
				sgn_sat = false;
				chosen_node = node;
			}
		}

	}

}



void dpll_choose_node_visitor::clause_event(
	const graph& g, vertex_descriptor clause, const vert_prop& prop) {

}



void dpll_choose_node_visitor::reset() {

	max_num_clauses_sat = 0;
	sgn_sat = false;
	chosen_node = vertex_descriptor();

}
