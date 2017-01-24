#pragma once

#include "sat/graph.h"
#include "sat/assignment/assignment.h"
#include "prune_stack.h"



namespace sat {

	class problem;

	class dpll_formula {

	private:

		graph partial_graph;
		incomplete_assignment partial_assign;
		// Could also put this in dpll_solver or in local function scope
		prune_stack prune_action_stack;

	public:

		explicit dpll_formula(const problem& prob);

		void set_node(vertex_descriptor node, bool value);

		void reverse_prune_to_node(vertex_descriptor node);

	};

}
