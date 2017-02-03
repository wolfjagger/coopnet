#include "testing.h"
#include "coopnet/sat/problem.h"
#include "coopnet/sat/generation/problem_factory.h"



namespace testing {

	sat::problem solvable_problem(bool assignment_sgn) {

		return sat::generate_solvable_3sat_problem(
			10, 100, assignment_sgn);

	}

	sat::problem two_solvable_problems(bool assignment_sgn) {

		return sat::generate_disconnected_solvable_3sat_problem(
			5, 5, 50, 50, assignment_sgn);

	}

	sat::problem random_problem() {

		return sat::generate_random_3sat_problem(28, 100);

	}

}
