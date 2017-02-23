#include <numeric>
#include "catch.hpp"
#include "coopnet/sat/problem/shuffler.h"
#include "coopnet/sat/solving/dpll/dpll_solver.h"
#include "prob.gen.h"
#include "rc_printing.h"

using namespace sat;



TEST_CASE("Problem initialization", "[sat]") {
	
	SECTION("Problem graph has correct size after init.") {

		auto lam = [](const Problem& prob) {

			RC_ASSERT(prob.get_graph().m_vertices.size()
				== prob.get_num_nodes() + prob.get_num_clauses());

		};

		REQUIRE(rc::check(lam));

	}

}



namespace {

	auto lam_create_same_sgn_assign
		= [](const Problem& prob, bool assignment_sgn) {

		auto assign = prob.create_same_sgn_assignment(assignment_sgn);
		RC_ASSERT(prob.is_satisfied_by(assign));
		return assign;

	};

	// Here and below: Add CHOICE for node_choice_mode.
	//  Choose at high level!
	auto lam_dpll_solve_satisfiable = [](const Problem& prob) {

		auto solver = sat::DPLLSolver(DPLLNodeChoiceMode::Next);
		auto pair = solver.solve(prob);

		RC_ASSERT(pair.first == sat::SolutionStatus::Satisfied);

		auto assign_dpll = pair.second;
		RC_ASSERT(assign_dpll);

		RC_ASSERT(prob.is_satisfied_by(assign_dpll));

	};

	auto lam_dpll_unknown_all = [](const Problem& prob) {

		auto solver_next = sat::DPLLSolver(DPLLNodeChoiceMode::Next);
		auto pair_next = solver_next.solve(prob);
		auto solver_rand = sat::DPLLSolver(DPLLNodeChoiceMode::Random);
		auto pair_rand = solver_rand.solve(prob);
		auto solver_most_sat = sat::DPLLSolver(DPLLNodeChoiceMode::MostClausesSat);
		auto pair_most_sat = solver_most_sat.solve(prob);

		RC_ASSERT(pair_next.first == pair_rand.first);
		RC_ASSERT(pair_next.first == pair_most_sat.first);

		switch (pair_next.first) {
		case sat::SolutionStatus::Satisfied:
			RC_ASSERT(prob.is_satisfied_by(pair_next.second));
			RC_ASSERT(prob.is_satisfied_by(pair_rand.second));
			RC_ASSERT(prob.is_satisfied_by(pair_most_sat.second));
			break;
		case sat::SolutionStatus::Unsatisfiable:
			// Note this does not assure it is truly unsatisfiable
			//  if dpll says it is.
			break;
		case sat::SolutionStatus::Undetermined:
			RC_FAIL();
			break;
		}

	};

}

TEST_CASE("Problem assignment verification", "[sat]") {

	SECTION("Connected, same sign problem satisfiable; dpll solver finds solution.") {

		auto lam = [](bool assignment_sgn) {

			auto prob = *rc::same_sgn_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50),
				assignment_sgn);

			lam_create_same_sgn_assign(prob, assignment_sgn);

			lam_dpll_solve_satisfiable(prob);

		};

		REQUIRE(rc::check(lam));
		
	}

	SECTION("Connected, same sign disconnected problem satisfiable; dpll solver finds solution.") {

		auto lam = [](bool assignment_sgn) {

			auto prob = *rc::same_sgn_disconnected_prob_gen(
				std::make_pair(3, 6),
				std::make_pair(1, 30),
				std::make_pair(3, 6),
				std::make_pair(1, 30),
				assignment_sgn);

			lam_create_same_sgn_assign(prob, assignment_sgn);

			lam_dpll_solve_satisfiable(prob);

		};
		
		REQUIRE(rc::check(lam));
		
	}

	SECTION("Random problems give correct assignments if solvable and node choice doesn't matter.") {
		
		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_dpll_unknown_all(prob);

		};

		REQUIRE(rc::check(lam));

	}

}



TEST_CASE("Literal shuffle", "[sat]") {

	SECTION("problem invariants unaffected by literal shuffling") {

		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			auto shuffler = sat::LiteralShuffler(prob);

			shuffler.apply_to_problem(prob);

			RC_ASSERT(prob.get_graph().m_vertices.size()
				== prob.get_num_nodes() + prob.get_num_clauses());

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Literal shuffling does not affect problem/assignment satisfaction.") {

		auto lam = []() {

			auto prob = *rc::same_sgn_prob_gen(
				std::make_pair(7, 10),
				std::make_pair(10, 50),
				true);

			auto assign = lam_create_same_sgn_assign(prob, true);
			auto assign_cpy = *assign;

			auto shuffler = sat::LiteralShuffler(prob);

			shuffler.apply_to_problem(prob);
			shuffler.apply_to_assignment(*assign);
			RC_ASSERT(prob.is_satisfied_by(assign));

			if (shuffler.flips_sgns()) {
				RC_ASSERT(*assign != assign_cpy);
			} else {
				RC_ASSERT(*assign == assign_cpy);
			}

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Shuffling does not change DPLL solving.") {

		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(10, 50));

			auto shuffler = sat::LiteralShuffler(prob);

			auto solver = DPLLSolver(DPLLNodeChoiceMode::Next);

			using namespace rc::detail;

			auto solution_pair1 = solver.solve(prob);
			shuffler.apply_to_problem(prob);
			auto solution_pair2 = solver.solve(prob);

			RC_ASSERT(
				solution_pair1.first == solution_pair2.first);

		};

		REQUIRE(rc::check(lam));

	}

}
