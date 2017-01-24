#pragma once

#include "sat/solving/solver.h"
#include "sat/graph.h"
#include "dpll_formula.h"



namespace sat {

	class dpll_solver : public complete_solver {

	private:

		dpll_formula formula;
	
	protected:
		
		virtual solve_return do_solve(const problem& prob) override;

	private:

		solve_return partial_solve();

	};

}
