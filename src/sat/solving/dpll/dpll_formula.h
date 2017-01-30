#pragma once

#include <functional>
#include "sat/graph.h"
#include "dpll_visitor.h"



namespace sat {

	class problem;

	class dpll_formula {

	private:

		std::reference_wrapper<const graph> prob_graph;

		bool contradicting;
		// Could also put this in dpll_solver or in local function scope
		prune_stack prune_action_stack;
		// Queue for remaining grey nodes to color black
		boost::queue<vertex_descriptor> grey_buffer;

		// Assignment information, including "unassigned"
		incomplete_assignment partial_assign;
		// For property maps associated with vert/edge statuses
		dpll_vert_status_map vert_status_map;
		dpll_edge_status_map edge_status_map;
		// Vert color map primarily controlled dynamically by visitor
		dpll_color_map color_map;

		dpll_prop_maps prop_maps;

		std::unique_ptr<dpll_visitor> prune_visitor;

	public:

		explicit dpll_formula(const problem& prob);

		dpll_formula(const dpll_formula& other);
		dpll_formula& operator=(const dpll_formula& other);

		dpll_formula(dpll_formula&& other) = default;
		dpll_formula& operator=(dpll_formula&& other) = default;

		~dpll_formula() = default;



		void set_node(vertex_descriptor node, bool first_choice);

		void reverse_prune_to_assignment(vertex_descriptor node);

		const incomplete_assignment& get_incomplete_assignment() const {
			return partial_assign;
		}
		incomplete_assignment& get_incomplete_assignment() {
			return partial_assign;
		}

		bool is_SAT() const;

		bool is_contradicting() const {
			return prune_visitor->is_contradicting;
		}
		void set_contradicting(bool value) {
			prune_visitor->is_contradicting = value;
		}

	};

}
