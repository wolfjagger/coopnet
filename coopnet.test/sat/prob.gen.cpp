#include "prob.gen.h"
#include "coopnet/sat/problem/problem_factory.h"
#include "coopnet/sat/problem/problem_util.h"
#include "sat.gen.h"



using namespace rc;

namespace {

	using ClauseList = std::set<sat::Clause>;



	struct ProbGenerationStats {
		unsigned int min_num_nodes = 3;
		unsigned int max_num_nodes = 10;
		unsigned int min_clause_size = 3;
		unsigned int max_clause_size = 3;
		unsigned int min_num_clauses = 1;
		unsigned int max_num_clauses = 50;
	};



	Gen<sat::Clause> create_clause_gen(
		const ProbGenerationStats& stats,
		unsigned int num_nodes) {

		auto clause_len_gen
			= gen::inRange<size_t>(stats.min_clause_size, stats.max_clause_size + 1);
		return gen::mapcat(clause_len_gen,
			[num_nodes](size_t len) {

			// Node generator, with 0 <= id < size
			auto node_gen = node_gen_with_int_gen(
				gen::inRange<unsigned int>(0, num_nodes));

			return clause_gen_with_nodes(len, node_gen);

		});

	}



	Gen<ClauseList> create_clause_list_gen(
		const ProbGenerationStats& stats,	unsigned int num_nodes) {

		// Assume all same length
		auto min_num_clauses = stats.min_num_clauses;

		auto min_clause_size = stats.min_clause_size;
		if (num_nodes < min_clause_size) min_clause_size = num_nodes;
		auto max_clause_size = stats.max_clause_size;
		if (num_nodes < max_clause_size) max_clause_size = num_nodes;

		auto max_num_clauses
			= static_cast<unsigned int>(
				sat::problem_util::max_num_clauses_with_lengths(
					num_nodes, min_clause_size, max_clause_size));

		if (stats.max_num_clauses < max_num_clauses)
			max_num_clauses = stats.max_num_clauses;
		if (max_num_clauses < min_num_clauses)
			min_num_clauses = max_num_clauses;

		auto num_clause_gen
			= gen::inRange<unsigned int>(
				min_num_clauses, max_num_clauses+1);

		return gen::mapcat(num_clause_gen,
			[num_nodes, &stats](unsigned int num_clauses) {

			auto clause_gen = create_clause_gen(stats, num_nodes);

			return gen::container<ClauseList>(num_clauses, clause_gen);

		});

	}



	GenProb create_problem_with_num_nodes(
		const ProbGenerationStats& stats, unsigned int num_nodes) {

		// Map the result of clause_max generator to clause generator
		auto clause_list_gen = create_clause_list_gen(stats, num_nodes);

		// Map clause generator to problem generator
		return gen::map(clause_list_gen,
			[num_nodes](std::set<sat::Clause>& set) {

			return sat::Problem(num_nodes, set.cbegin(), set.cend());

		});

	}
			
	GenProb create_problem(const ProbGenerationStats stats) {

		// Generator for number of nodes
		auto int_gen = gen::inRange(stats.min_num_nodes, stats.max_num_nodes+1);

		// Concatenate number of nodes to give problem generator
		return gen::mapcat(int_gen, [stats](unsigned int size) {

			return create_problem_with_num_nodes(stats, size);

		});

	}

}



namespace rc {

	GenProb Arbitrary<sat::Problem>::arbitrary() {

		ProbGenerationStats stats;
		return create_problem(stats);

	}



	Gen<sat::Problem> same_sgn_prob_gen(
		MinMax num_nodes, MinMax num_clauses,
		bool assignment_sgn) {
	
		auto num_nodes_gen = rc::gen::inRange<unsigned int>(
			num_nodes.first, num_nodes.second + 1);
		return gen::mapcat(num_nodes_gen,
			[num_clauses, assignment_sgn](unsigned int num_nodes) {
			
			auto num_clauses_gen = rc::gen::inRange<unsigned int>(
				num_clauses.first, num_clauses.second + 1);
			return gen::map(num_clauses_gen,
				[num_nodes, assignment_sgn](unsigned int num_clauses) {

				return sat::generate_solvable_3sat_problem(
					num_nodes, num_clauses, assignment_sgn);

			});

		});

	}


	GenProb same_sgn_disconnected_prob_gen(
		MinMax num_nodes1, MinMax num_clauses1,
		MinMax num_nodes2, MinMax num_clauses2,
		bool assignment_sgn) {

		auto num_nodes_gen1 = rc::gen::inRange<unsigned int>(
			num_nodes1.first, num_nodes1.second+1);
		auto num_clauses_gen1 = rc::gen::inRange<unsigned int>(
			num_clauses1.first, num_clauses1.second+1);
		auto num_nodes_gen2 = rc::gen::inRange<unsigned int>(
			num_nodes2.first, num_nodes2.second+1);
		auto num_clauses_gen2 = rc::gen::inRange<unsigned int>(
			num_clauses2.first, num_clauses2.second+1);
		auto sgn_gen = rc::gen::just(assignment_sgn);

		return gen::apply(
			sat::generate_disconnected_solvable_3sat_problem,
			num_nodes_gen1, num_nodes_gen2,
			num_clauses_gen1, num_clauses_gen2, sgn_gen);

	}
		
	
	GenProb random_prob_gen(
		MinMax num_nodes, MinMax num_clauses) {

		auto num_nodes_gen = rc::gen::inRange<unsigned int>(
			num_nodes.first, num_nodes.second+1);
		auto num_clauses_gen = rc::gen::inRange<unsigned int>(
			num_clauses.first, num_clauses.second+1);

		return gen::apply(
			sat::generate_random_3sat_problem,
			num_nodes_gen, num_clauses_gen);

	}

}
