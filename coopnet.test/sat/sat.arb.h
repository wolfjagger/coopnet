#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "sat/component/node.h"
#include "sat/assignment/assignment.h"



namespace rc {

	template<>
	struct Arbitrary<sat::node> {
		static Gen<sat::node> arbitrary() {
			return gen::construct<sat::node>(
				gen::arbitrary<unsigned int>());
		}
	};

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

			// This shows how to restrict things if necessary
			/*auto num_pred = [](sat::assignment::map& data) {
				return data.size() < 100;
			};

			return gen::build<sat::assignment>(
				gen::set(&sat::assignment::data,
					gen::suchThat<sat::assignment::map>(num_pred)));*/

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

	namespace detail {
		
		std::ostream& operator<<(std::ostream& os, const sat::node& n) {
			return os << n.id;
		}

		std::ostream& operator<<(std::ostream& os, const boost::logic::tribool& b) {
			if (b) {
				os << "T";
			} else if (!b) {
				os << "F";
			} else {
				os << "I";
			}
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const sat::assignment& assign) {
			os << "assignment:" << std::endl;
			for (auto iter = assign.data.cbegin();
				iter != assign.data.cend(); ++iter) {
				os << " k" << iter->first << " v" << iter->second;
			}
			return os << std::endl;
		}
		
		std::ostream& operator<<(std::ostream& os,
			const sat::incomplete_assignment& assign) {
			os << "incomplete_assignment:" << std::endl;
			for (auto iter = assign.data.cbegin();
				iter != assign.data.cend(); ++iter) {
				os << " k" << iter->first << " v" << iter->second;
			}
			return os << std::endl;
		}
	}

}
