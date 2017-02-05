#include <numeric>
#include "catch.hpp"
#include "prob.arb.h"
#include "coopnet/sat/generation/problem_factory.h"

using namespace sat;



TEST_CASE("Problem initialization", "[sat]") {
	
	SECTION("Problem graph has correct size after init.") {

		auto lam = [](const problem& prob) {

			RC_ASSERT(prob.get_graph().m_vertices.size()
				== prob.get_num_nodes() + prob.get_num_clauses());

		};

		REQUIRE(rc::check(lam));

	}

}

TEST_CASE("Problem assignment verification", "[sat]") {

	auto lam_gen_prob = [](bool assignment_sgn) {
		return sat::generate_solvable_3sat_problem(
			10, 100, assignment_sgn);
	};

	auto lam_gen_assignment = [](const sat::problem& prob, bool assignment_sgn) {
		// Should be satisfied by an assignment of all true
		std::map<sat::vertex_descriptor, bool> map_assign;
		for (auto& pair : prob.get_map_node_to_vert()) {
			map_assign.emplace(pair.second, assignment_sgn);
		}
		auto assign = std::make_shared<sat::assignment>();
		assign->data = std::move(map_assign);
		return assign;
	};

	auto lam_tot = [lam_gen_prob, lam_gen_assignment](bool assignment_sgn) {
		auto prob = lam_gen_prob(assignment_sgn);
		auto assign = lam_gen_assignment(prob, assignment_sgn);
		return prob.is_satisfied_by(assign);
	};

	SECTION("Connected, all true problem satisfiable.") {

		REQUIRE(lam_tot(true));
		
	}

	SECTION("Connected, all false problem satisfiable.") {

		REQUIRE(lam_tot(false));

	}

}
