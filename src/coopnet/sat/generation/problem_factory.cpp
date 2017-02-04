#include "problem_factory.h"
#include "alphali/util/random.h"
#include "coopnet/sat/problem.h"


namespace sat {

	using alphali::take_chance;

	constexpr auto escape_num = 20;

	namespace {

		problem generate_3sat_problem(
			unsigned num_nodes, unsigned num_clauses,
			float prob0, float prob1, float prob2) {

			// 3-SAT and expecting no duplicates, so need at least three literals.
			if (num_nodes < 3)
				throw std::exception("Not enough nodes to construct a clause.");

			auto clauses = std::vector<clause>();

			for (unsigned int clause_idx = 0; clause_idx < num_clauses; ++clause_idx) {

				// Generate a clause by getting three distinct nodes randomly
				//  and flip a coin for each literals sgn.
				auto nums = alphali::rand_vec_leq(num_nodes-1, 3);
				clauses.emplace_back(clause::lit_storage{
					literal(unsigned(nums[0]), take_chance(prob0)),
					literal(unsigned(nums[1]), take_chance(prob1)),
					literal(unsigned(nums[2]), take_chance(prob2)),
				});

			}

			return problem(num_nodes, clauses.begin(), clauses.end());

		}

		problem generate_disconnected_3sat_problem(
			unsigned num_nodes1, unsigned num_nodes2,
			unsigned num_clauses1, unsigned num_clauses2,
			float prob0, float prob1, float prob2) {

			if (num_nodes1 < 3 || num_nodes2 < 3)
				throw std::exception("Not enough nodes to construct a clause.");

			auto clauses = std::vector<sat::clause>();

			for (unsigned int clause_idx = 0; clause_idx < num_clauses1; ++clause_idx) {

				auto nums = alphali::rand_vec_leq(num_nodes1-1, 3);
				clauses.emplace_back(clause::lit_storage{
					literal(unsigned(nums[0]), take_chance(prob0)),
					literal(unsigned(nums[1]), take_chance(prob1)),
					literal(unsigned(nums[2]), take_chance(prob2)),
				});

			}

			for (unsigned int clause_idx = 0; clause_idx < num_clauses2; ++clause_idx) {

				auto nums = alphali::rand_vec_inclusive(
					num_nodes1, num_nodes1 + num_nodes2 - 1, 3);
				clauses.emplace_back(clause::lit_storage{
					literal(unsigned(nums[0]), take_chance(prob0)),
					literal(unsigned(nums[1]), take_chance(prob1)),
					literal(unsigned(nums[2]), take_chance(prob2)),
				});

			}

			return problem(num_nodes1+num_nodes2,
				clauses.begin(), clauses.end());

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
