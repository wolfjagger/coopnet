#pragma once

#include "rapidcheck.h"
#include "coopnet/sat/problem/problem.h"



namespace rc {

	enum class clause_length_type {
		Fixed, LessThan
	};

	using minmax = std::pair<unsigned int, unsigned int>;
	
	using Gen_prob = Gen<sat::problem>;



	template<>
	struct Arbitrary<sat::problem> {
		static Gen_prob arbitrary();
	};

	Gen_prob same_sgn_prob_gen(
		minmax num_nodes, minmax num_clauses,
		bool assignment_sgn);

	Gen_prob same_sgn_disconnected_prob_gen(
		minmax num_nodes1, minmax num_clauses1,
		minmax num_nodes2, minmax num_clauses2,
		bool assignment_sgn);
		
	Gen_prob random_prob_gen(
		minmax num_nodes, minmax num_clauses);

}
