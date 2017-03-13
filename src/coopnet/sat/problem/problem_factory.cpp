#include "problem_factory.h"
#include <algorithm>
#include "alphali/util/random.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "problem.h"
#include "problem_util.h"



namespace coopnet {

	using alphali::take_chance;

	constexpr auto escape_num = 20;

	namespace {

		unsigned int calc_max_num_clauses(
			unsigned int num_nodes, unsigned int num_clauses,
			unsigned int clause_len = 3) {

			// 3-SAT and expecting no duplicates, so need at least three literals.
			if (num_nodes < clause_len)
				throw std::exception("Not enough nodes to construct a clause.");

			auto max_num_clauses
				= static_cast<unsigned int>(
					problem_util::max_num_clauses_with_length(num_nodes, 3));

			return std::min(max_num_clauses, num_clauses);

		}


		Problem generate_3sat_problem(
			unsigned int num_nodes, unsigned int num_clauses,
			float prob0, float prob1, float prob2) {

			auto max_num_clauses
				= calc_max_num_clauses(num_nodes, num_clauses);

			auto clauses = std::vector<Clause>();

			for (unsigned int clause_idx = 0;
				clause_idx < max_num_clauses; ++clause_idx) {

				// Generate a clause by getting three distinct nodes randomly
				//  and flip a coin for each literals sgn.
				auto nums = alphali::rand_vec_leq(num_nodes-1, 3);
				clauses.emplace_back(Clause{
					Literal(unsigned(nums[0]), take_chance(prob0)),
					Literal(unsigned(nums[1]), take_chance(prob1)),
					Literal(unsigned(nums[2]), take_chance(prob2)),
				});

			}

			return Problem(num_nodes, clauses.begin(), clauses.end());

		}

		Problem generate_disconnected_3sat_problem(
			unsigned num_nodes1, unsigned num_nodes2,
			unsigned num_clauses1, unsigned num_clauses2,
			float prob0, float prob1, float prob2) {

			auto clauses = std::vector<coopnet::Clause>();

			auto max_num_clauses1
				= calc_max_num_clauses(num_nodes1, num_clauses1);

			for (unsigned int clause_idx = 0;
				clause_idx < max_num_clauses1; ++clause_idx) {

				auto nums = alphali::rand_vec_leq(num_nodes1-1, 3);
				clauses.emplace_back(Clause{
					Literal(unsigned(nums[0]), take_chance(prob0)),
					Literal(unsigned(nums[1]), take_chance(prob1)),
					Literal(unsigned(nums[2]), take_chance(prob2)),
				});

			}

			auto max_num_clauses2
				= calc_max_num_clauses(num_nodes2, num_clauses2);

			for (unsigned int clause_idx = 0;
				clause_idx < max_num_clauses2; ++clause_idx) {

				auto nums = alphali::rand_vec_inclusive(
					num_nodes1, num_nodes1 + num_nodes2 - 1, 3);
				clauses.emplace_back(Clause{
					Literal(unsigned(nums[0]), take_chance(prob0)),
					Literal(unsigned(nums[1]), take_chance(prob1)),
					Literal(unsigned(nums[2]), take_chance(prob2)),
				});

			}

			return Problem(num_nodes1+num_nodes2,
				clauses.begin(), clauses.end());

		}

	}

	Problem generate_solvable_3sat_problem(
		unsigned num_nodes, unsigned num_clauses,
		bool assignment_sgn) {

		auto prob1 = assignment_sgn ? 1.f : 0.f;
		return generate_3sat_problem(
			num_nodes, num_clauses, prob1, 0.5, 0.5);

	}

	Problem generate_disconnected_solvable_3sat_problem(
		unsigned num_nodes1, unsigned num_nodes2,
		unsigned num_clauses1, unsigned num_clauses2,
		bool assignment_sgn) {

		auto prob1 = assignment_sgn ? 1.f : 0.f;
		return generate_disconnected_3sat_problem(
			num_nodes1, num_nodes2, num_clauses1, num_clauses2, prob1, 0.5, 0.5);

	}

	Problem generate_random_3sat_problem(unsigned num_nodes, unsigned num_clauses) {

		return generate_3sat_problem(num_nodes, num_clauses, 0.5, 0.5, 0.5);

	}

	Problem generate_disconnected_random_3sat_problem(
		unsigned num_nodes1, unsigned num_nodes2,
		unsigned num_clauses1, unsigned num_clauses2) {

		return generate_disconnected_3sat_problem(
			num_nodes1, num_nodes2, num_clauses1, num_clauses2, 0.5, 0.5, 0.5);

	}

}
