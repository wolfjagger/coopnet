#pragma once

#include "sat/solving/solver.h"



namespace sat {

	class tracer_solver : public complete_solver {

	protected:

		solve_return do_solve(const problem& prob) override;

	};

}
