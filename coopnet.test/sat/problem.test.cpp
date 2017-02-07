#include <numeric>
#include "catch.hpp"
#include "prob.gen.h"
#include "sat.gen.h"
#include "coopnet/sat/generation/problem_factory.h"
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

	//TODO: Replace with problem generators (in coopnet.test),
	// and firm up num_nodes/num_clauses relations (in coopnet)
	auto lam_gen_prob = [](bool assignment_sgn) {
		auto num_nodes = *rc::gen::inRange<unsigned int>(3, 10);
		auto num_clauses = *rc::gen::inRange<unsigned int>(
			1, 2 * num_nodes*num_nodes);
		return sat::generate_solvable_3sat_problem(
			num_nodes, num_clauses, assignment_sgn);
	};

	auto lam_gen_disconnected_prob = [](bool assignment_sgn) {
		auto num_nodes = *rc::gen::inRange<unsigned int>(3, 6);
		auto num_clauses = *rc::gen::inRange<unsigned int>(
			1, 2 * num_nodes*num_nodes);
		return sat::generate_disconnected_solvable_3sat_problem(
			num_nodes, num_nodes, num_clauses, num_clauses, assignment_sgn);
	};

	auto lam_gen_random_prob = []() {
		auto num_nodes = *rc::gen::inRange<unsigned int>(3, 10);
		auto hard_num_clauses = unsigned int(std::round(num_nodes * 4.2f));
		auto num_clauses = *rc::gen::inRange<unsigned int>(
			hard_num_clauses-num_nodes, hard_num_clauses+num_nodes);
		return sat::generate_random_3sat_problem(num_nodes, num_clauses);
	};

	auto lam_check_solvable = [](const problem& prob, bool assignment_sgn) {

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

		auto lam = [lam_gen_prob, lam_check_solvable](bool assignment_sgn) {

			auto prob = lam_gen_prob(assignment_sgn);
			lam_check_solvable(prob, assignment_sgn);

		};

		REQUIRE(rc::check(lam));
		
	}

	SECTION("Connected, same sign problem satisfiable; dpll solver finds solution.") {

		auto lam = [lam_gen_disconnected_prob, lam_check_solvable](bool assignment_sgn) {

			auto prob = lam_gen_disconnected_prob(assignment_sgn);
			lam_check_solvable(prob, assignment_sgn);

		};

		REQUIRE(rc::check(lam));
		
	}

	SECTION("Random problems give correct assignments if solvable.") {
		
		auto lam = [lam_gen_random_prob]() {

			auto prob = lam_gen_random_prob();
		
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
