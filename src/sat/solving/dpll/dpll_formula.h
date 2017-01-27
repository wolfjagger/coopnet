#pragma once

#include <functional>
#include "sat/graph.h"
#include "dpll_visitor.h"



namespace sat {

	class problem;

	class dpll_formula {

	private:

		std::reference_wrapper<const graph> prob_graph;
		incomplete_assignment partial_assign;
		// Could also put this in dpll_solver or in local function scope
		prune_stack prune_action_stack;

		// For property maps associated with vert/edge statuses
		//  and the vert color that is controlled by visitor
		dpll_vert_status_map vert_status_map;
		dpll_edge_status_map edge_status_map;
		dpll_color_map color_map;

		std::unique_ptr<dpll_visitor> visitor;

	public:

		explicit dpll_formula(const problem& prob);

		dpll_formula(const dpll_formula& other);
		dpll_formula& operator=(const dpll_formula& other);

		dpll_formula(dpll_formula&& other) = default;
		dpll_formula& operator=(dpll_formula&& other) = default;

		~dpll_formula() = default;



		void set_node(vertex_descriptor node, bool value);

		void reverse_prune_to_node(vertex_descriptor node);

	};

}
