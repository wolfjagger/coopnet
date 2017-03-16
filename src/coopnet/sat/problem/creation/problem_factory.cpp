#include "problem_factory.h"
#include <algorithm>
#include "alphali/util/random.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/problem/problem_util.h"



namespace coopnet { namespace problem_factory {

	using alphali::take_chance;

	constexpr auto escapeNum = 20;

	namespace {

		unsigned int calc_max_num_clauses(
			unsigned int numNodes, unsigned int numClauses,
			unsigned int clauseLen = 3) {

			// 3-SAT and expecting no duplicates, so need at least three literals.
			if (numNodes < clauseLen)
				throw std::exception("Not enough nodes to construct a clause.");

			auto maxNumClauses
				= static_cast<unsigned int>(
					problem_util::max_num_clauses_with_length(numNodes, 3));

			return std::min(maxNumClauses, numClauses);

		}


		template<class LitVecGenerator>
		Problem gen_problem(
			unsigned int numNodes, unsigned int numClauses,
			LitVecGenerator litVecGen) {

			auto maxNumClauses
				= calc_max_num_clauses(numNodes, numClauses);

			auto clauses = std::vector<Clause>();

			for (unsigned int clauseIdx = 0;
				clauseIdx < maxNumClauses; ++clauseIdx) {

				auto lits = litVecGen();
				clauses.emplace_back(Clause(lits.begin(), lits.end()));

			}

			return Problem(numNodes, clauses.begin(), clauses.end());

		}


		template<class LitVecGenerator1, class LitVecGenerator2>
		Problem gen_disconnected_problem(
			unsigned numNodes1, unsigned numNodes2,
			unsigned numClauses1, unsigned numClauses2,
			LitVecGenerator1 litVecGen1, LitVecGenerator2 litVecGen2) {

			auto clauses = std::vector<coopnet::Clause>();

			auto maxNumClauses1
				= calc_max_num_clauses(numNodes1, numClauses1);

			for (unsigned int clauseIdx = 0;
				clauseIdx < maxNumClauses1; ++clauseIdx) {

				auto lits = litVecGen1();
				clauses.emplace_back(Clause(lits.begin(), lits.end()));

			}

			auto maxNumClauses2
				= calc_max_num_clauses(numNodes2, numClauses2);

			for (unsigned int clauseIdx = 0;
				clauseIdx < maxNumClauses2; ++clauseIdx) {

				auto lits = litVecGen2();
				clauses.emplace_back(Clause(lits.begin(), lits.end()));

			}

			return Problem(numNodes1+numNodes2,
				clauses.begin(), clauses.end());

		}

	}



	Problem solvable_3sat_problem(
		unsigned numNodes, unsigned numClauses,
		bool assignmentSgn) {

		// First node always solvable with assignmentSgn
		auto prob0 = assignmentSgn ? 1.0 : 0.0;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		auto litVecGen = [numNodes, prob0, prob1, prob2]() {
			auto nums = alphali::rand_vec_leq(numNodes - 1, 3);
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};
		};

		return gen_problem(numNodes, numClauses, litVecGen);

	}

	Problem disconnected_solvable_3sat_problem(
		unsigned numNodes1, unsigned numNodes2,
		unsigned numClauses1, unsigned numClauses2,
		bool assignmentSgn) {

		// First node always solvable with assignmentSgn
		auto prob0 = assignmentSgn ? 1.0 : 0.0;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		auto litVecGen1 = [numNodes1, prob0, prob1, prob2]() {
			auto nums = alphali::rand_vec_leq(numNodes1 - 1, 3);
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};
		};

		auto litVecGen2 = [numNodes1, numNodes2, prob0, prob1, prob2]() {
			auto nums = alphali::rand_vec_inclusive(
				numNodes1, numNodes1 + numNodes2 - 1, 3);
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};
		};

		return gen_disconnected_problem(
			numNodes1, numNodes2, numClauses1, numClauses2,
			litVecGen1, litVecGen2);

	}



	Problem random_3sat_problem(unsigned numNodes, unsigned numClauses) {

		// All nodes have random sgn
		auto prob0 = 0.5;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		auto litVecGen = [numNodes, prob0, prob1, prob2]() {
			auto nums = alphali::rand_vec_leq(numNodes - 1, 3);
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};
		};

		return gen_problem(numNodes, numClauses, litVecGen);

	}

	Problem disconnected_random_3sat_problem(
		unsigned numNodes1, unsigned numNodes2,
		unsigned numClauses1, unsigned numClauses2) {

		// All nodes have random sgn
		auto prob0 = 0.5;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		auto litVecGen1 = [numNodes1, prob0, prob1, prob2]() {
			auto nums = alphali::rand_vec_leq(numNodes1 - 1, 3);
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};
		};

		auto litVecGen2 = [numNodes1, numNodes2, prob0, prob1, prob2]() {
			auto nums = alphali::rand_vec_inclusive(
				numNodes1, numNodes1 + numNodes2 - 1, 3);
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};
		};

		return gen_disconnected_problem(
			numNodes1, numNodes2, numClauses1, numClauses2,
			litVecGen1, litVecGen2);

	}



	Problem random_3sat_problem_with_1D_metric(
		unsigned numNodes, unsigned numClauses,
		double stdDev) {

		// All nodes have random sgn
		auto prob0 = 0.5;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		// Give each node an "x value" between 0 and 1,
		//  representing placement in metric space.
		using fcn_pair = std::pair<unsigned, double>;
		auto nodeXVec = std::vector<fcn_pair>();
		nodeXVec.reserve(numNodes);
		for(unsigned node=0; node<numNodes; ++node) {
			nodeXVec.emplace_back(node, alphali::unit_rand());
		}

		// Sort the vector by x value
		std::sort(nodeXVec.begin(), nodeXVec.end(), [](const fcn_pair& p1, const fcn_pair& p2) {
			return p1.second < p2.second;
		});

		auto litVecGen = [nodeXVec{std::move(nodeXVec)}, prob0, prob1, prob2]() {

			// Choose set of nodes based on distribution favoring clusters:
			//  P ~ exp(1/(2*stdDev) * (sum_i x_i^2 - 3*\mu_{x_1,x_2,x_3}^2))
			//auto nums = alphali::rand_vec_leq(numNodes - 1, 3);

			return std::vector<Literal> {
				/*Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))*/
			};

		};

		return gen_problem(numNodes, numClauses, litVecGen);

	}

} }
