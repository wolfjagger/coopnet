#include "catch.hpp"
#include "sat.gen.h"

using namespace sat;



TEST_CASE("Node", "[sat]") {

	SECTION("Comparisons.") {

		auto node0 = node(0);
		auto node1 = node(0);
		auto node2 = node(1);

		REQUIRE(node0 == node1);
		REQUIRE(node0 != node2);
		REQUIRE(node0 < node2);
		REQUIRE(node2 > node0);

	}

	SECTION("node_list contains no duplicates.") {
		
		constexpr auto num_nodes = 15;
		auto node_list = create_nodes(num_nodes);

		std::sort(node_list.begin(), node_list.end());

		REQUIRE(
			std::adjacent_find(node_list.cbegin(), node_list.cend())
			== node_list.cend());

	}


}



TEST_CASE("Clause", "[sat]") {

	SECTION("Comparisons.") {

		auto lam = [](
			sat::literal::lit_set& lits0,
			sat::literal::lit_set& lits1) {

			auto clause0 = clause(lits0.cbegin(), lits0.cend());
			auto clause1 = clause(lits1.cbegin(), lits1.cend());

			if(lits0 == lits1) {
				RC_ASSERT(clause0 == clause1);
			} else if (lits0 < lits1) {
				RC_ASSERT(clause0 != clause1);
				RC_ASSERT(clause0 < clause1);
				RC_ASSERT(clause1 > clause0);
			} else if (lits0 > lits1) {
				RC_ASSERT(clause0 != clause1);
				RC_ASSERT(clause0 > clause1);
				RC_ASSERT(clause1 < clause0);
			}
			
		};

		REQUIRE(rc::check(lam));

	}

}



TEST_CASE("Assignment", "[sat]") {

	SECTION("Assignment throws if initialized from incomplete_assignment with indeterminate value.") {

		rc::Gen<double> gen = rc::gen::just(0.0);

		auto lam = []() {

			auto partial_assign
				= *rc::gen::suchThat<sat::incomplete_assignment>(is_indeterminate);

			RC_ASSERT_THROWS(auto assign = sat::assignment(partial_assign));
			
		};

		REQUIRE(rc::check(lam));

	}

}
