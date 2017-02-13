#include "create_data.h"
#include "coopnet/sat/problem/problem_factory.h"



namespace coopgephi {

	sat::problem random_problem(int num_nodes, int num_clauses) {

		return sat::generate_random_3sat_problem(num_nodes, num_clauses);

	}

}
