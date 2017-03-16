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
	// Algorithm is random with a gaussian spread on the metric.
	Problem random_3sat_problem_with_1D_metric(
		unsigned numNodes, unsigned numClauses,
		double stdDev);

	// Generates 3SAT problem with preferential attachment, drawing inspiration from
	//  the Barabasi-Albert model: generate a small random graph then preferentially append.
	// We will instead select nodes for clauses in proportion to degree+1.
	Problem barabasi_albert_3sat_problem(
		unsigned numNodes, unsigned numClauses);

	// Generates 3SAT problem with large clustering, drawing inspiration from
	//  the Watts-Strogatz model: generate a circulant graph then rewire.
	// We will generate a 3sat problem with clauses connecting nearby nodes (circulant),
	//  then rewire each clause's edges with some probability.
	Problem watts_strogatz_3sat_problem(
		unsigned numNodes, unsigned numClauses,
		double rewire_chance);

} }
