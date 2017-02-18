#pragma once

#include <stack>
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/solving/solver.h"
#include "coopnet/graph/graph.h"
#include "dpll_node_chooser.h"



namespace sat {

	class dpll_formula;

	class dpll_solver : public complete_solver {

	private:

		std::unique_ptr<dpll_formula> formula;

		// Choice: node, sgn, first time chosen (out of two)
		std::stack<std::tuple<node, bool, bool>> choices;

	public:

		dpll_solver(dpll_node_choice_mode mode);

		dpll_solver(const dpll_solver& other) = delete;
		dpll_solver& operator=(const dpll_solver& other) = delete;

		dpll_solver(dpll_solver&& other) = default;
		dpll_solver& operator=(dpll_solver&& other) = default;

		~dpll_solver();
	
	protected:
		
		virtual solve_return do_solve(const problem& prob) override;

	private:

		void find_assignment();

		bool change_last_free_choice();
		void reduce_with_selection(node n, bool choice, bool is_first_time);

	};

}
