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



		auto default_lit_generator(
			unsigned numNodes, double prob0, double prob1, double prob2) {

			return [numNodes, prob0, prob1, prob2]() {
				auto nums = alphali::rand_vec_leq(numNodes - 1, 3);
				return std::vector<Literal> {
					Literal(unsigned(nums[0]), take_chance(prob0)),
					Literal(unsigned(nums[1]), take_chance(prob1)),
					Literal(unsigned(nums[2]), take_chance(prob2))
				};
			};

		}



		template<class LitVecGenerator>
		std::vector<Clause> gen_list_clauses(
			unsigned int numNodes, unsigned int numClauses,
			LitVecGenerator&& litVecGen) {

			auto maxNumClauses
				= calc_max_num_clauses(numNodes, numClauses);

			auto clauses = std::vector<Clause>();

			for (unsigned int clauseIdx = 0;
				clauseIdx < maxNumClauses; ++clauseIdx) {

				auto lits = litVecGen();
				clauses.emplace_back(Clause(lits.begin(), lits.end()));

			}

			return clauses;

		}

		template<class LitVecGenerator>
		Problem gen_problem(
			unsigned int numNodes, unsigned int numClauses,
			LitVecGenerator&& litVecGen) {

			auto clauses = gen_list_clauses(numNodes, numClauses, litVecGen);

			return Problem(numNodes, clauses.begin(), clauses.end());

		}


		template<class LitVecGenerator1, class LitVecGenerator2>
		Problem gen_disconnected_problem(
			unsigned numNodes1, unsigned numNodes2,
			unsigned numClauses1, unsigned numClauses2,
			LitVecGenerator1&& litVecGen1, LitVecGenerator2&& litVecGen2) {

			auto clauses1 = gen_list_clauses(numNodes1, numClauses1, litVecGen1);
			auto clauses2 = gen_list_clauses(numNodes2, numClauses2, litVecGen2);

			clauses1.insert(clauses1.end(), clauses2.begin(), clauses2.end());

			return Problem(numNodes1+numNodes2,
				clauses1.begin(), clauses1.end());

		}

	}



	Problem solvable_3sat_problem(
		unsigned numNodes, unsigned numClauses,
		bool assignmentSgn) {

		// First node always solvable with assignmentSgn
		auto prob0 = assignmentSgn ? 1.0 : 0.0;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		return gen_problem(numNodes, numClauses,
			default_lit_generator(numNodes, prob0, prob1, prob2));

	}

	Problem disconnected_solvable_3sat_problem(
		unsigned numNodes1, unsigned numNodes2,
		unsigned numClauses1, unsigned numClauses2,
		bool assignmentSgn) {

		// First node always solvable with assignmentSgn
		auto prob0 = assignmentSgn ? 1.0 : 0.0;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

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
			default_lit_generator(numNodes1, prob0, prob1, prob2),
			litVecGen2);

	}



	Problem random_3sat_problem(unsigned numNodes, unsigned numClauses) {

		// All nodes have random sgn
		auto prob0 = 0.5;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		return gen_problem(numNodes, numClauses,
			default_lit_generator(numNodes, prob0, prob1, prob2));

	}

	Problem disconnected_random_3sat_problem(
		unsigned numNodes1, unsigned numNodes2,
		unsigned numClauses1, unsigned numClauses2) {

		// All nodes have random sgn
		auto prob0 = 0.5;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

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
			default_lit_generator(numNodes1, prob0, prob1, prob2),
			litVecGen2);

	}



	// BROKEN
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
		std::sort(nodeXVec.begin(), nodeXVec.end(),
			[](const fcn_pair& p1, const fcn_pair& p2) {
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



	Problem barabasi_albert_3sat_problem(
		unsigned numNodes, unsigned numClauses) {

		// All nodes have random sgn
		auto prob0 = 0.5;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		// Set up a vector of node degrees
		auto nodeDegree = std::vector<unsigned>(numNodes);
		std::fill(nodeDegree.begin(), nodeDegree.end(), 0);
		auto totDegree = numNodes;

		// Generator for Barabasi-Albert is somewhat complicated.
		//  It takes a list of degrees and links clauses to nodes in proportion to (degree+1).
		auto litVecGen
			= [nodeDegree{ std::move(nodeDegree) }, totDegree, prob0, prob1, prob2]() mutable {

			// Generate three unique numbers
			auto nums = std::vector<int>(3);
			for (unsigned i = 0; i < nums.size(); ++i) {

				while (true) {
					
					// Accumulate node degrees (+1) until we reach a threshold.
					//  This should give preference in proportion to (degree+1).
					auto accumulatedDegree = 0;
					auto accumulatedDegreeEnd = alphali::rand_leq(totDegree - 1);

					auto newNum = 0;
					for (unsigned num = 0; num < nodeDegree.size(); ++num) {
						accumulatedDegree += nodeDegree[num] + 1;
						if (accumulatedDegree > accumulatedDegreeEnd) {
							newNum = num;
							break;
						}
					}

					// Check the new node is unique
					auto unique = true;
					for (auto j = 0; j < i; ++j) {
						if (newNum == nums[j]) unique = false;
					}

					if (unique) {
						nums[i] = newNum;
						break;
					}

				}

			}

			// Now update degrees for each of the nums.
			for(auto num : nums) {
				++nodeDegree[num];
				++totDegree;
			}

			// Finally, return the literal list.
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};

		};

		return gen_problem(numNodes, numClauses, litVecGen);

	}



	// Generates 3SAT problem with large clustering, drawing inspiration from
	//  the Watts-Strogatz model: generate a circulant graph then rewire.
	// We will generate a 3sat problem with clauses connecting nearby nodes (circulant),
	//  then rewire each clause's edges with some probability.
	Problem watts_strogatz_3sat_problem(
		unsigned numNodes, unsigned numClauses,
		double rewire_chance) {

		// All nodes have random sgn
		auto prob0 = 0.5;
		auto prob1 = 0.5;
		auto prob2 = 0.5;

		// Generate a list of clauses first that just give a ~circulant graph.
		auto ratio = double(numNodes)/numClauses;
		auto currentClause = 0;
		auto litVecGen
			= [currentClause, ratio, numNodes, prob0, prob1, prob2]() mutable {

			auto pseudoNode = currentClause*ratio;
			auto currentNode = unsigned(std::floor(pseudoNode));
			auto subNode = unsigned(std::floor((pseudoNode - currentNode) / ratio));

			// Link up currentNode, currentNode+1, currentNode+2+subNode
			auto nums = std::vector<unsigned>(3);
			nums[0] = currentNode;
			nums[1] = currentNode + alphali::rand_inclusive(1, subNode+1);
			nums[2] = currentNode + subNode+2;

			// Fix problems: >= numNodes and duplicates
			for (auto numIter = nums.begin(); numIter != nums.end(); ++numIter) {

				auto& num = *numIter;
				auto needToCheck = true;
				while(needToCheck) {

					needToCheck = false;

					// If too large, wrap around
					while (num >= numNodes) {
						num -= numNodes;
						needToCheck = true;
					}

					// If already in clause before, index by 1
					while (std::find(nums.begin(), numIter, num) != numIter) {
						num += 1;
						needToCheck = true;
					}

				}

			}

			// Go to next clause
			++currentClause;

			// Finally, return the literal list.
			return std::vector<Literal> {
				Literal(unsigned(nums[0]), take_chance(prob0)),
				Literal(unsigned(nums[1]), take_chance(prob1)),
				Literal(unsigned(nums[2]), take_chance(prob2))
			};

		};

		// Generate circulant clauses
		auto clauses = gen_list_clauses(numNodes, numClauses, litVecGen);

		// Now for each node in the clause, give a chance to rewire
		for (auto& c : clauses) {
			auto& lits = c.literals();
			for (auto& lit : lits) {
				
				auto rand = alphali::unit_rand();
				if (rand <= rewire_chance) {

					auto& oldNode = lit.first.id;

					auto newNode = alphali::rand_leq(numNodes-1);
					while (newNode != oldNode && std::any_of(lits.begin(), lits.end(),
						[newNode](std::pair<Node, bool> l) { return l.first.id == newNode; })) {
						newNode = alphali::rand_leq(numNodes-1);
					}

					oldNode = newNode;

				}

			}
		}

		return Problem(numNodes, clauses.begin(), clauses.end());

	}

} }
