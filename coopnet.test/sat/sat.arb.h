#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/assignment/assignment.h"
#include "rc_printing.h"



namespace rc {

	// Nodes
	inline Gen<sat::node> node_gen_with_int_gen(
		Gen<unsigned int>& int_gen) {
		return gen::construct<sat::node>(int_gen);
	}

	template<>
	struct Arbitrary<sat::node> {
		static Gen<sat::node> arbitrary() {
			return gen::construct<sat::node>(
				gen::arbitrary<unsigned int>());
		}
	};

	// Literals
	inline Gen<sat::literal> lit_gen_with_nodes(
		Gen<sat::node>& node_gen) {

		return gen::construct<sat::literal>(
			node_gen, gen::arbitrary<bool>());

	}

	template<>
	struct Arbitrary<sat::literal> {
		static Gen<sat::literal> arbitrary() {
			return lit_gen_with_nodes(gen::arbitrary<sat::node>());
		}
	};

	// Clause
	inline Gen<sat::clause> clause_gen_with_nodes(
		Gen<sat::node>& node_gen) {

		auto lit_list_gen = gen::uniqueBy<std::vector<sat::literal>>(
			lit_gen_with_nodes(node_gen),
			[](const sat::literal& lit) {
			return lit.n;
		});
		return gen::construct<sat::clause>(lit_list_gen);

	}

	template<>
	struct Arbitrary<sat::clause> {
		static Gen<sat::clause> arbitrary() {
			return clause_gen_with_nodes(gen::arbitrary<sat::node>());
		}
	};

	// Tribool
	template<>
	struct Arbitrary<boost::logic::tribool> {
		static Gen<boost::logic::tribool> arbitrary() {

			return gen::map(gen::inRange(0, 3), [](int choice) {
				if (choice == 0) {
					return boost::logic::tribool(true);
				} else if (choice == 1) {
					return boost::logic::tribool(boost::indeterminate);
				} else {
					return boost::logic::tribool(false);
				}
			});

		}
	};

	// Assignment
	template<>
	struct Arbitrary<sat::assignment> {
		static Gen<sat::assignment> arbitrary() {

			return gen::build<sat::assignment>(
				gen::set(&sat::assignment::data));

		}
	};

	template<>
	struct Arbitrary<sat::incomplete_assignment> {
		static Gen<sat::incomplete_assignment> arbitrary() {

			return gen::build<sat::incomplete_assignment>(
				gen::set(&sat::incomplete_assignment::data,
					gen::container<sat::incomplete_assignment::map>(
						gen::arbitrary<sat::vertex_descriptor>(),
						gen::arbitrary<boost::logic::tribool>())));

		}
	};

}
