#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "coopnet/sat/problem.h"
#include "sat.arb.h"



namespace rc {

	template<>
	struct Arbitrary<sat::problem> {
		static Gen<sat::problem> arbitrary() {

			// Generator for number of nodes
			auto int_gen = gen::inRange(1, 20);

			// Concatenate number of nodes to give problem generator
			return gen::mapcat(int_gen, [](unsigned int size) {

				// Generator for max clause length (1 <= len < size+1)
				auto max_clause_size = size;
				if (max_clause_size > 5) max_clause_size = 5;
				auto clause_max_gen
					= gen::inRange<unsigned int>(1, max_clause_size + 1);

				// Map the result of clause_max generator to clause generator
				auto clause_list_gen
					= gen::mapcat(clause_max_gen, [size](unsigned int max_len) {

					// Assume no more than three nodes per clause
					auto max_num_clauses = 2 * size;
					if (max_len > 1) {
						max_num_clauses += 4 * size*(size - 1) / 2;
					}
					if (max_len > 2) {
						max_num_clauses += 8 * size*(size - 1)*(size - 2) / 6;
					}
					if (max_num_clauses > 100) max_num_clauses = 100;

					auto num_clause_gen
						= gen::inRange<unsigned int>(1, max_num_clauses);

					return gen::mapcat(num_clause_gen,
						[size, max_len](unsigned int num_clauses) {

						auto clause_len_gen
							= gen::inRange<size_t>(1, max_len + 1);
						auto clause_gen = gen::mapcat(clause_len_gen,
							[size](size_t len) {

							// Node generator, with 0 <= id < size
							auto node_gen = node_gen_with_int_gen(
								gen::inRange<unsigned int>(0, size));

							return clause_gen_with_nodes(len, node_gen);

						});

						return gen::container<std::set<sat::clause>>(
							num_clauses, clause_gen);

					});

				});

				// Map clause generator to problem generator
				return gen::map(clause_list_gen,
					[size](std::set<sat::clause>& set) {

					return sat::problem(size, set.cbegin(), set.cend());

				});

			});

		}
	};

}
