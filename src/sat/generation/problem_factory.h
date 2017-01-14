#pragma once



namespace sat {
	
	class problem;

	problem generate_solvable_3sat_problem(unsigned num_nodes, unsigned num_clauses);
	problem generate_random_3sat_problem(unsigned num_nodes, unsigned num_clauses);

}
