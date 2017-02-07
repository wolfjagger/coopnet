#pragma once



namespace sat {
	
	class problem;

	problem generate_solvable_3sat_problem(
		unsigned num_nodes, unsigned num_clauses,
		bool assignment_sgn);
	problem generate_disconnected_solvable_3sat_problem(
		unsigned num_nodes1, unsigned num_nodes2,
		unsigned num_clauses1, unsigned num_clauses2,
		bool assignment_sgn);
	problem generate_random_3sat_problem(
		unsigned num_nodes, unsigned num_clauses);
	problem generate_disconnected_random_3sat_problem(
		unsigned num_nodes1, unsigned num_nodes2,
		unsigned num_clauses1, unsigned num_clauses2);

}
