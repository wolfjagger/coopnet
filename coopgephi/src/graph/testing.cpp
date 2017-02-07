#include "testing.h"
#include "coopnet/sat/problem.h"
#include "coopnet/sat/generation/problem_factory.h"



namespace testing {

	sat::problem random_problem() {

		return sat::generate_random_3sat_problem(28, 100);

	}

}
