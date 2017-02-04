#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "coopnet/sat/problem.h"
#include "sat.arb.h"



namespace rc {

	template<>
	struct Arbitrary<sat::problem> {
		static Gen<sat::problem> arbitrary() {

			auto int_gen = gen::inRange(1, 20);

			return gen::mapcat(int_gen, [](unsigned int size) {

				auto clause_list_gen
					= gen::unique<std::vector<sat::clause>>(
						clause_gen_with_nodes(
							node_gen_with_int_gen(
								gen::inRange<unsigned int>(0, size))));

				return gen::map(clause_list_gen,
					[size](std::vector<sat::clause>& vec) {

					return sat::problem(size, vec.cbegin(), vec.cend());

				});

			});

		}
	};

}
