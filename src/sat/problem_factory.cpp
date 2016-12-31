#include "problem_factory.h"
#include "problem.h"
#include "util/random.h"


namespace sat {

	problem generate_random_3sat_problem(unsigned num_nodes, unsigned num_clauses) {

		using sat::clause_data;
		using jj_random::coin_flip;

		constexpr auto escape_num = 20;

		// 3-SAT and expecting no duplicates, so need at least three literals.
		if (num_nodes < 3) throw std::exception("Not enough nodes to construct a clause.");

		auto clause_init = std::vector<clause_data>();

		for(unsigned int clause_idx = 0; clause_idx < num_clauses; ++clause_idx) {

			// Generate a clause by first getting three distinct nodes randomly...
			auto nums = jj_random::rand_vec_less_than(num_nodes, 3);
			auto nodes = clause_data::node_storage{ nums[0], nums[1], nums[2] };
			// ...and then flip a coin for each literals sgn.
			auto sgns = clause_data::sgn_storage{ coin_flip(), coin_flip(), coin_flip() };
			clause_init.emplace_back(nodes, sgns);

		}

		return problem(num_nodes, clause_init.begin(), clause_init.end());

	}

}
