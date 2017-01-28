#pragma once

#include "sat/solving/solver.h"
#include "sat/graph.h"
#include "dpll_formula.h"



namespace sat {

	class dpll_solver : public complete_solver {

	public:

		// Note: node choice will include sort by largest connection,
		//  watched literals or clauses, most clauses solved, etc.
		enum class node_choice_mode {
			Next, Last, Random
		};

	private:

		dpll_formula formula;
	
	protected:
		
		virtual solve_return do_solve(const problem& prob) override;

	private:

		solve_return partial_solve();

		vertex_descriptor choose_next_node(node_choice_mode mode) const;

	};

}
