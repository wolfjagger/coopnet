#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/assignment/assignment.h"
#include "rc_printing.h"



namespace rc {

	template<>
	struct Arbitrary<sat::node> {
		static Gen<sat::node> arbitrary() {
			return gen::construct<sat::node>(
				gen::arbitrary<unsigned int>());
		}
	};

	inline Gen<sat::node> node_gen_within(
		unsigned int min_id, unsigned int max_id) {
		return gen::construct<sat::node>(gen::inRange(min_id, max_id));
	}

	template<>
	struct Arbitrary<sat::clause> {
		static Gen<sat::clause> arbitrary() {

			return gen::exec([]() {
				auto node_list = *gen::unique<std::vector<sat::node>>(
					gen::arbitrary<sat::node>());
				auto sgn_list = *gen::container<std::vector<bool>>(
					node_list.size(), gen::arbitrary<bool>());
				return sat::clause(sat::clause_data(node_list, sgn_list));
			});

		}
	};

	inline Gen<sat::clause> clause_gen_from_nodes(
		unsigned int min_id, unsigned int max_id) {

		return gen::exec([min_id, max_id]() {
			auto node_list = *gen::unique<std::vector<sat::node>>(
				node_gen_within(min_id, max_id));
			auto sgn_list = *gen::container<std::vector<bool>>(
				node_list.size(), gen::arbitrary<bool>());
			return sat::clause(sat::clause_data(node_list, sgn_list));
		});

	}

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
