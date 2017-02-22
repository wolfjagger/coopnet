#include "catch.hpp"
#include "sat.gen.h"

using namespace sat;



TEST_CASE("Node", "[sat]") {

	SECTION("Comparisons.") {

		auto node0 = Node(0);
		auto node1 = Node(0);
		auto node2 = Node(1);

		REQUIRE(node0 == node1);
		REQUIRE(node0 != node2);
		REQUIRE(node0 < node2);
		REQUIRE(node2 > node0);

	}

	SECTION("node_list contains no duplicates.") {
		
		constexpr auto num_nodes = 15;
		auto NodeList = create_nodes(num_nodes);

		std::sort(NodeList.begin(), NodeList.end());

		REQUIRE(
			std::adjacent_find(NodeList.cbegin(), NodeList.cend())
			== NodeList.cend());

	}


}



TEST_CASE("Clause", "[sat]") {

	SECTION("Comparisons.") {

		auto lam = [](
			sat::Literal::LitSet& lits0,
			sat::Literal::LitSet& lits1) {

			auto clause0 = Clause(lits0.cbegin(), lits0.cend());
			auto clause1 = Clause(lits1.cbegin(), lits1.cend());

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



/*TEST_CASE("Assignment", "[sat]") {

	SECTION("Assignment throws if initialized from incomplete_assignment with indeterminate value.") {

		rc::Gen<double> gen = rc::gen::just(0.0);

		auto lam = []() {

			auto partial_assign
				= *rc::gen::suchThat<sat::IncompleteAssignment>(is_ind_assignment);

			RC_ASSERT_THROWS(auto assign = sat::Assignment(partial_assign));
			
		};

		REQUIRE(rc::check(lam));

	}

}*/
