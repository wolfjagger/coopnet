#pragma once



namespace coopnet {
	class Problem;
}

namespace coopnet { namespace problem_factory {
	
	// Generates 3SAT problem that is solvable with all assignmentSgn nodes
	Problem solvable_3sat_problem(
		unsigned numNodes, unsigned numClauses,
		bool assignmentSgn);
	// Generates disconnected 3SAT problem that is solvable with all assignmentSgn nodes
	Problem disconnected_solvable_3sat_problem(
		unsigned numNodes1, unsigned numNodes2,
		unsigned numClauses1, unsigned numClauses2,
		bool assignmentSgn);

	// Generates Erdos-Renyi random 3SAT problem
	Problem random_3sat_problem(
		unsigned numNodes, unsigned numClauses);
	// Generates disconnected Erdos-Renyi random 3SAT problem
	Problem disconnected_random_3sat_problem(
		unsigned numNodes1, unsigned numNodes2,
		unsigned numClauses1, unsigned numClauses2);

	// Generates 3SAT problem that assigns a value from the 1D metric range [0, 1]
	//  to nodes and preferentially connects nodes with close values on this range.
	// Algorithm is Erdos-Renyi with a gaussian spread on the metric.
	Problem random_3sat_problem_with_1D_metric(
		unsigned numNodes, unsigned numClauses,
		double stdDev);

} }
