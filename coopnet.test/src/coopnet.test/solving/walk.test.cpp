#include <numeric>
#include "catch.hpp"
#include "coopnet/sat/solving/walk/walk_solver.h"
#include "coopnet.test/prob.gen.h"
#include "coopnet.test/rc_printing.h"

using namespace coopnet;



namespace {

	void check_solution(const Problem& prob, const Solution& solution) {

		switch (solution.status) {
		case SolutionStatus::Satisfied:
			RC_ASSERT(prob.is_satisfied_by(solution.assignment));
			break;
		case SolutionStatus::Partial:
			RC_FAIL();
			break;
		case SolutionStatus::Unsatisfied:
			RC_FAIL();
			break;
		case SolutionStatus::Undetermined:
			break;
		}

	}

	auto lam_walk_unknown_all = [](const Problem& prob, unsigned int numTries) {

		auto solver_rand = WalkSolver(numTries, WalkNodeChoiceMode::Random);
		auto pair_rand = solver_rand.solve(prob);
		check_solution(prob, pair_rand);

		auto solver_gsat = WalkSolver(numTries, WalkNodeChoiceMode::GSAT);
		auto pair_gsat = solver_gsat.solve(prob);
		check_solution(prob, pair_gsat);

	};

}

TEST_CASE("WalkSAT", "[sat]") {

	SECTION("Random problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_walk_unknown_all(prob, 5);

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Barabasi-Albert problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::barabasi_albert_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_walk_unknown_all(prob, 5);

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Watts-Strogatz problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::watts_strogatz_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_walk_unknown_all(prob, 5);

		};

		REQUIRE(rc::check(lam));

	}

}
