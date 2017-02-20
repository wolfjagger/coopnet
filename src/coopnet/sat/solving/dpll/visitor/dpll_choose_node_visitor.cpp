#include "dpll_choose_node_visitor.h"
#include <iostream>

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



DPLLChooseNodeVisitor::DPLLChooseNodeVisitor(DPLLPropMaps init_maps) :
	maps(init_maps) {

	reset();

}



void DPLLChooseNodeVisitor::node_event(
	const SatGraph& g, VertDescriptor node, const VertProp& prop) {

	if(maps.vert_status_map[node] == DPLLVertStatus::Active) {

		if (DEBUG) std::cout << "node " << node << " Active" << std::endl;

		int num_pos_edges = 0;
		int num_neg_edges = 0;

		auto edge_pair = boost::out_edges(node, g);
		for (auto edge_iter = edge_pair.first;
			edge_iter != edge_pair.second; ++edge_iter) {

			if (maps.edge_status_map[*edge_iter] == DPLLEdgeStatus::Active) {

				if (g[*edge_iter].sgn) {
					++num_pos_edges;
				} else {
					++num_neg_edges;
				}

			}

		}

		if (DEBUG) {
			std::cout << "num_pos_edges: " << std::to_string(num_pos_edges) << std::endl;
			std::cout << "num_neg_edges: " << std::to_string(num_neg_edges) << std::endl;
		}

		if (num_pos_edges >= num_neg_edges) {
			if(num_pos_edges > max_num_clauses_sat) {

				if (DEBUG) {
					std::cout << "Max num clauses sat: " << std::to_string(num_pos_edges);
					std::cout << " with sgn true." << std::endl;
				}

				max_num_clauses_sat = num_pos_edges;
				*chosen_node = node;
				*sgn_sat = true;

			}
		} else {
			if (num_neg_edges > max_num_clauses_sat) {

				if(DEBUG) {
					std::cout << "Max num clauses sat: " << std::to_string(num_neg_edges);
					std::cout << " with sgn false." << std::endl;
				}

				max_num_clauses_sat = num_neg_edges;
				*chosen_node = node;
				*sgn_sat = false;

			}
		}

	}

}



void DPLLChooseNodeVisitor::clause_event(
	const SatGraph& g, VertDescriptor clause, const VertProp& prop) {

}



void DPLLChooseNodeVisitor::reset() {

	max_num_clauses_sat = -1;
	chosen_node = std::make_shared<VertDescriptor>();
	sgn_sat = std::make_shared<bool>(false);

}



std::pair<VertDescriptor, bool> DPLLChooseNodeVisitor::retreive_choice() const {
	return std::make_pair(*chosen_node, *sgn_sat);
}
