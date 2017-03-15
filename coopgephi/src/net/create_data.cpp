#include "create_data.h"
#include "coopnet/sat/problem/creation/problem_factory.h"



namespace coopgephi {

	coopnet::Problem random_problem(int num_nodes, int num_clauses) {

		return coopnet::generate_random_3sat_problem(num_nodes, num_clauses);

	}

}
