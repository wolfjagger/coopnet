#include "testing.h"
#include "sat/problem.h"
#include "sat/generation/problem_factory.h"



namespace testing {

	sat::problem test_problem() {

		return sat::generate_random_3sat_problem(10, 100);

	}

}
