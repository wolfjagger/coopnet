#include "problem_factory.h"
#include "alphali/util/random.h"
#include "sat/problem.h"


namespace sat {

	using sat::clause_data;
	using alphali::take_chance;

	constexpr auto escape_num = 20;

	namespace {

		problem generate_3sat_problem(
			unsigned num_nodes, unsigned num_clauses,
			float prob1, float prob2, float prob3) {

			// 3-SAT and expecting no duplicates, so need at least three literals.
			if (num_nodes < 3)
				throw std::exception("Not enough nodes to construct a clause.");

			auto clause_init = std::vector<clause_data>();

			for (unsigned int clause_idx = 0; clause_idx < num_clauses; ++clause_idx) {

				// Generate a clause by first getting three distinct nodes randomly...
				auto nums = alphali::rand_vec_leq(num_nodes-1, 3);
				auto nodes = clause_data::node_storage{
					unsigned(nums[0]), unsigned(nums[1]), unsigned(nums[2])
				};
				// ...and then flip a coin for each literals sgn.
				auto sgns = clause_data::sgn_storage{
					take_chance(prob1), take_chance(prob2), take_chance(prob3)
				};
				clause_init.emplace_back(nodes, sgns);

			}

			return problem(num_nodes, clause_init.begin(), clause_init.end());

		}

		problem generate_disconnected_3sat_problem(
			unsigned num_nodes1, unsigned num_nodes2,
			unsigned num_clauses1, unsigned num_clauses2,
			float prob1, float prob2, float prob3) {

			if (num_nodes1 < 3 || num_nodes2 < 3)
				throw std::exception("Not enough nodes to construct a clause.");

			auto clause_init = std::vector<clause_data>();

			for (unsigned int clause_idx = 0; clause_idx < num_clauses1; ++clause_idx) {

				auto nums = alphali::rand_vec_leq(num_nodes1-1, 3);
				auto nodes = clause_data::node_storage{
					unsigned(nums[0]), unsigned(nums[1]), unsigned(nums[2])
				};
				auto sgns = clause_data::sgn_storage{
					take_chance(prob1), take_chance(prob2), take_chance(prob3)
				};
				clause_init.emplace_back(nodes, sgns);

			}

			for (unsigned int clause_idx = 0; clause_idx < num_clauses2; ++clause_idx) {

				auto nums = alphali::rand_vec_inclusive(
					num_nodes1, num_nodes1 + num_nodes2 - 1, 3);
				auto nodes = clause_data::node_storage{
					unsigned(nums[0]), unsigned(nums[1]), unsigned(nums[2])
				};
				auto sgns = clause_data::sgn_storage{
					take_chance(prob1), take_chance(prob2), take_chance(prob3)
				};
				clause_init.emplace_back(nodes, sgns);

			}

			return problem(num_nodes1+num_nodes2,
				clause_init.begin(), clause_init.end());

		}

	}

	problem generate_solvable_3sat_problem(
		unsigned num_nodes, unsigned num_clauses,
		bool assignment_sgn) {

		auto prob1 = assignment_sgn ? 1.f : 0.f;
		return generate_3sat_problem(
			num_nodes, num_clauses, prob1, 0.5, 0.5);

	}

	problem generate_disconnected_solvable_3sat_problem(
		unsigned num_nodes1, unsigned num_nodes2,
		unsigned num_clauses1, unsigned num_clauses2,
		bool assignment_sgn) {

		auto prob1 = assignment_sgn ? 1.f : 0.f;
		return generate_disconnected_3sat_problem(
			num_nodes1, num_nodes2, num_clauses1, num_clauses2, prob1, 0.5, 0.5);

	}

	problem generate_random_3sat_problem(unsigned num_nodes, unsigned num_clauses) {

		return generate_3sat_problem(num_nodes, num_clauses, 0.5, 0.5, 0.5);

	}

	problem generate_disconnected_random_3sat_problem(
		unsigned num_nodes1, unsigned num_nodes2,
		unsigned num_clauses1, unsigned num_clauses2) {

		return generate_disconnected_3sat_problem(
			num_nodes1, num_nodes2, num_clauses1, num_clauses2, 0.5, 0.5, 0.5);

	}

}
