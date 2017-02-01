#include "catch.hpp"
#include "sat.arb.h"
#include "sat/component/clause.h"

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
			std::vector<std::pair<node, bool>>& nb1,
			std::vector<std::pair<node, bool>>& nb2) {

			auto nodes1 = std::vector<node>();
			auto sgns1 = std::vector<bool>();
			auto nodes2 = std::vector<node>();
			auto sgns2 = std::vector<bool>();

			for (auto& p : nb1) {
				nodes1.push_back(p.first);
				sgns1.push_back(p.second);
			}

			for (auto& p : nb2) {
				nodes2.push_back(p.first);
				sgns2.push_back(p.second);
			}

			auto clause1 = clause(clause_data(nodes1, sgns1));
			auto clause2 = clause(clause_data(nodes2, sgns2));

			if (nodes1 < nodes2) RC_ASSERT(clause1 < clause2);
			else if (nodes1 > nodes2) RC_ASSERT(clause1 > clause2);
			else {
				if (sgns1 < sgns2) RC_ASSERT(clause1 < clause2);
				else if(sgns1 > sgns2) RC_ASSERT(clause1 > clause2);
				else RC_ASSERT(clause1 == clause2);
			}
			
		};

		REQUIRE(rc::check(lam));

	}

}



TEST_CASE("Assignment", "[sat]") {

	SECTION("Assignment throws if initialized from incomplete_assignment with indeterminate value.") {

		auto lam = [](sat::incomplete_assignment& partial_assign) {

			auto ind_pred = [](sat::incomplete_assignment::pair p) {
				return boost::indeterminate(p.second);
			};

			// If partial_assign doesn't have an indeterminate value,
			//  manufacture one
			if (std::none_of(
				partial_assign.data.cbegin(),
				partial_assign.data.cend(),
				ind_pred)) {

				partial_assign.data.insert_or_assign(
					*rc::gen::arbitrary<sat::vertex_descriptor>(),
					boost::indeterminate);

			}

			RC_ASSERT_THROWS(auto assign = sat::assignment(partial_assign));
			
		};

		REQUIRE(rc::check(lam));

	}

}
