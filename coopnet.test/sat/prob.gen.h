#pragma once

#include "rapidcheck.h"
#include "coopnet/sat/problem/problem.h"



namespace rc {

	enum class ClauseLengthType {
		Fixed, LessThan
	};

	using MinMax = std::pair<unsigned int, unsigned int>;
	
	using GenProb = Gen<coopnet::Problem>;



	template<>
	struct Arbitrary<coopnet::Problem> {
		static GenProb arbitrary();
	};

	GenProb same_sgn_prob_gen(
		MinMax num_nodes, MinMax num_clauses,
		bool assignment_sgn);

	GenProb same_sgn_disconnected_prob_gen(
		MinMax num_nodes1, MinMax num_clauses1,
		MinMax num_nodes2, MinMax num_clauses2,
		bool assignment_sgn);
		
	GenProb random_prob_gen(
		MinMax num_nodes, MinMax num_clauses);

}
