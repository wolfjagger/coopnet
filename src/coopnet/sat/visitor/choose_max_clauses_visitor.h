#pragma once

#include <iostream>
#include "pruned_sat_visitor.h"



namespace coopnet {

	class ChooseMaxClausesVisitor :
		public PruneSatVertVisitor<ChooseMaxClausesVisitor> {

	public:
		using event_filter = boost::on_examine_vertex;

	private:

		static constexpr bool DEBUG = false;

		std::shared_ptr<VertDescriptor> chosen_node;
		std::shared_ptr<bool> sgn_sat;

		int max_num_clauses_sat;

	public:

		ChooseMaxClausesVisitor();



		template<class SatGraph, typename VProp>
		void node_event(
			const SatGraph& g, VertDescriptor node,
			const VProp& prop) {

			if (DEBUG) std::cout << "node " << node << std::endl;

			int num_pos_edges = 0;
			int num_neg_edges = 0;


			auto count_fcn = [&g, &num_pos_edges, &num_neg_edges](EdgeDescriptor e) {
				if (g[e].base.sgn) {
					++num_pos_edges;
				} else {
					++num_neg_edges;
				}
			};

			auto edge_pair = boost::out_edges(node, g);
			for_each_active_edge(node, g, count_fcn);


			if (DEBUG) {
				std::cout << "num_pos_edges: " << std::to_string(num_pos_edges) << std::endl;
				std::cout << "num_neg_edges: " << std::to_string(num_neg_edges) << std::endl;
			}

			if (num_pos_edges >= num_neg_edges) {
				if (num_pos_edges > max_num_clauses_sat) {

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

					if (DEBUG) {
						std::cout << "Max num clauses sat: " << std::to_string(num_neg_edges);
						std::cout << " with sgn false." << std::endl;
					}

					max_num_clauses_sat = num_neg_edges;
					*chosen_node = node;
					*sgn_sat = false;

				}
			}

		}



		template<class SatGraph, typename VProp>
		void clause_event(
			const SatGraph& g, VertDescriptor clause,
			const VProp& prop) { }



		void reset();



		std::pair<VertDescriptor, bool> retreive_choice() const;

	};

}
