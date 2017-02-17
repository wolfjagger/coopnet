#pragma once

#include <stack>
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/solving/solver.h"
#include "coopnet/graph/graph.h"



namespace sat {

	class dpll_formula;
	class dpll_node_chooser;

	class dpll_solver : public complete_solver {

	public:

		enum class node_choice_mode {
			Next, Last, Random
		};

	private:

		std::unique_ptr<dpll_formula> formula;
		std::unique_ptr<dpll_node_chooser> node_chooser;
		std::stack<std::pair<node, bool>> choices;

	public:

		dpll_solver(node_choice_mode mode);
	
	protected:
		
		virtual solve_return do_solve(const problem& prob) override;

	private:

		void find_assignment();

		bool change_last_free_choice();
		void reduce_with_selection(node n, bool choice);

	};

}
