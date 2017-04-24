#include <numeric>
#include "catch.hpp"
#include "coopnet/sat/solving/dpll/dpll_solver.h"
#include "coopnet.test/prob.gen.h"
#include "coopnet.test/rc_printing.h"

using namespace coopnet;



namespace {

	auto lam_dpll_unknown_all = [](const Problem& prob) {

		auto solver_next = DPLLSolver();
		solver_next.set_problem(prob);
		solver_next.create_chooser<NextNodeChooser>();
		auto pair_next = solver_next.solve();

		auto solver_rand = DPLLSolver();
		solver_rand.set_problem(prob);
		solver_rand.create_chooser<RandNodeChooser>();
		auto pair_rand = solver_rand.solve();

		auto solver_most_same_sat = DPLLSolver();
		solver_most_same_sat.set_problem(prob);
		solver_most_same_sat.create_chooser<MaxSameClauseNodeChooser>();
		auto pair_most_same_sat = solver_most_same_sat.solve();

		auto solver_most_tot_sat = DPLLSolver();
		solver_most_tot_sat.set_problem(prob);
		solver_most_tot_sat.create_chooser<MaxTotClauseNodeChooser>();
		auto pair_most_tot_sat = solver_most_tot_sat.solve();

		RC_ASSERT(pair_next.status == pair_rand.status);
		RC_ASSERT(pair_next.status == pair_most_same_sat.status);
		RC_ASSERT(pair_next.status == pair_most_tot_sat.status);

		switch (pair_next.status) {
		case SolutionStatus::Satisfied:
			RC_ASSERT(prob.is_satisfied_by(pair_next.assignment));
			RC_ASSERT(prob.is_satisfied_by(pair_rand.assignment));
			RC_ASSERT(prob.is_satisfied_by(pair_most_same_sat.assignment));
			RC_ASSERT(prob.is_satisfied_by(pair_most_tot_sat.assignment));
			break;
		case SolutionStatus::Partial:
			RC_FAIL();
			break;
		case SolutionStatus::Unsatisfied:
			// Note this does not assure it is truly unsatisfiable
			//  if dpll says it is.
			break;
		case SolutionStatus::Undetermined:
			RC_FAIL();
			break;
		}

	};

}

TEST_CASE("DPLLSAT", "[sat]") {

	SECTION("Random problems give correct assignments if solvable, and node choice doesn't matter.") {
		
		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_dpll_unknown_all(prob);

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Barabasi-Albert problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::barabasi_albert_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_dpll_unknown_all(prob);

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Watts-Strogatz problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::watts_strogatz_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_dpll_unknown_all(prob);

		};

		REQUIRE(rc::check(lam));

	}

}
