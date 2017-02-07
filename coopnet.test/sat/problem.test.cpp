#include <numeric>
#include "catch.hpp"
#include "prob.gen.h"
#include "sat.gen.h"
#include "coopnet/sat/solving/dpll/dpll_solver.h"

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

	auto lam_check_solvable = [](const problem& prob, bool assignment_sgn, bool TMP = false) {

		auto assign = prob.create_same_sgn_assignment(assignment_sgn);
		RC_ASSERT(prob.is_satisfied_by(assign));

		auto solver = sat::dpll_solver();
		auto pair = solver.solve(prob);

		RC_ASSERT(pair.first == sat::solution_status::Satisfied);

		auto assign_dpll = pair.second;
		RC_ASSERT(assign_dpll);
		RC_ASSERT(prob.is_satisfied_by(assign_dpll));

	};



	SECTION("Connected, same sign problem satisfiable; dpll solver finds solution.") {

		auto lam = [lam_check_solvable](bool assignment_sgn) {

			auto prob = *rc::same_sgn_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 40),
				assignment_sgn);

			lam_check_solvable(prob, assignment_sgn);

		};

		REQUIRE(rc::check(lam));
		
	}

	SECTION("Connected, same sign disconnected problem satisfiable; dpll solver finds solution.") {

		auto lam = [lam_check_solvable](bool assignment_sgn) {

			auto prob = *rc::same_sgn_disconnected_prob_gen(
				std::make_pair(3, 6),
				std::make_pair(1, 20),
				std::make_pair(3, 6),
				std::make_pair(1, 20),
				assignment_sgn);

			lam_check_solvable(prob, assignment_sgn);

		};
		
		REQUIRE(rc::check(lam));
		
	}

	SECTION("Random problems give correct assignments if solvable.") {
		
		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));
		
			auto solver = sat::dpll_solver();
			auto pair = solver.solve(prob);
			switch (pair.first) {
			case sat::solution_status::Satisfied:
				RC_ASSERT(prob.is_satisfied_by(pair.second));
				break;
			case sat::solution_status::Unsatisfiable:
				// Note this does not assure it is truly unsatisfiable
				//  if dpll says it is.
				break;
			case sat::solution_status::Undetermined:
				RC_FAIL();
				break;
			}

		};

		REQUIRE(rc::check(lam));

	}

}
