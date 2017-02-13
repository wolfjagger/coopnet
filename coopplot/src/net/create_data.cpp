#include "create_data.h"
#include "coopnet/sat/problem/problem_factory.h"



namespace coopplot {

	sat::problem random_problem() {

		return sat::generate_random_3sat_problem(28, 100);

	}

}