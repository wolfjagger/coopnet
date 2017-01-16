#pragma once

#include "solver.h"
#include "sat/component/assignment.h"
#include "sat/graph.h"



namespace sat {

	class dpll_solver : public complete_solver {

	private:

		graph partial_graph;
		assignment partial_assign;
	
	protected:
		
		virtual solve_return do_solve(const problem& prob) override;

	private:

		solve_return partial_solve();

		void remove_unit_clauses();
		void remove_pure_literals();
		
	};

}
