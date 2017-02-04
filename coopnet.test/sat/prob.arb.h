#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "coopnet/sat/problem.h"
#include "sat.arb.h"



namespace rc {

	template<>
	struct Arbitrary<sat::problem> {
		static Gen<sat::problem> arbitrary() {

			return gen::exec([]() {
				auto size = *gen::inRange(1, 20);
				auto clause_list = *gen::unique<std::vector<sat::clause>>(
					clause_gen_with_nodes(
						node_gen_with_int_gen(
							gen::inRange<unsigned int>(0, size))));
				return sat::problem(
					size, clause_list.begin(), clause_list.end());
			});

		}
	};

}