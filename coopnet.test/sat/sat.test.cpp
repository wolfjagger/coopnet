#include "catch.hpp"
#include "rapidcheck.h"
#include "sat/component/node.h"
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

		auto node0 = node(0);
		auto node1 = node(1);
		auto node2 = node(2);
		auto node3 = node(3);

		auto vec_node = std::vector<node>({ node0, node1, node2 });
		auto vec_sgn = std::vector<bool>({ true, true, false });
		auto clause1 = clause(clause_data(vec_node, vec_sgn));
		auto clause2 = clause(clause_data(vec_node, vec_sgn));

		vec_sgn = std::vector<bool>({ true, false, false });
		auto clause3 = clause(clause_data(vec_node, vec_sgn));

		vec_node = std::vector<node>({ node0, node1, node3 });
		vec_sgn = std::vector<bool>({ true, false, false });
		auto clause4 = clause(clause_data(vec_node, vec_sgn));

		REQUIRE(clause1 == clause2);
		REQUIRE(clause1 != clause3);
		REQUIRE(clause1 != clause4);
		REQUIRE(clause3 != clause4);
		REQUIRE(clause1 > clause3);
		REQUIRE(clause1 < clause4);
		REQUIRE(clause3 < clause4);

	}

}
