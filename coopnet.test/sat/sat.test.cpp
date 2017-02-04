#include "catch.hpp"
#include "sat.arb.h"

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
			std::vector<sat::literal>& lits1,
			std::vector<sat::literal>& lits2) {

			auto clause1 = clause(lits1.cbegin(), lits1.cend());
			auto clause2 = clause(lits2.cbegin(), lits2.cend());

			if (lits1 < lits2) RC_ASSERT(clause1 < clause2);
			
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
