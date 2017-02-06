#include <numeric>
#include "catch.hpp"
#include "prob.gen.h"
#include "sat.gen.h"
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

	auto lam_gen_prob = [](
		unsigned int num_nodes, unsigned int num_clauses, bool assignment_sgn) {

		return sat::generate_solvable_3sat_problem(
			num_nodes, num_clauses, assignment_sgn);

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
		auto num_nodes = *rc::gen::inRange<unsigned int>(3, 20);
		auto num_clauses = *rc::gen::inRange<unsigned int>(
			1, 2 * num_nodes*num_nodes);
		auto prob = lam_gen_prob(num_nodes, num_clauses, assignment_sgn);
		auto assign = lam_gen_assignment(prob, assignment_sgn);
		return prob.is_satisfied_by(assign);
	};

	SECTION("Connected, same sign problem satisfiable.") {

		auto lam = [lam_tot](bool assignment_sgn) {
			return lam_tot(assignment_sgn);
		};

		REQUIRE(rc::check(lam));
		
	}
}
