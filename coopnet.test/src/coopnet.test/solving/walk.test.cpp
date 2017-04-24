#include <numeric>
#include "catch.hpp"
#include "coopnet/sat/solving/walk/walk_solver.h"
#include "coopnet.test/prob.gen.h"
#include "coopnet.test/rc_printing.h"

using namespace coopnet;



namespace {

	constexpr bool DEBUG = false;

	bool check_solution(const Problem& prob, const Solution& solution) {

		switch (solution.status) {
		case SolutionStatus::Satisfied:
			if (DEBUG) std::cout << "numSteps: " << solution.numSteps << std::endl;
			RC_ASSERT(prob.is_satisfied_by(solution.assignment));
			return true;
		case SolutionStatus::Partial:
			RC_FAIL();
			return false;
		case SolutionStatus::Unsatisfied:
			RC_FAIL();
			return false;
		case SolutionStatus::Undetermined:
			return false;
		default:
			throw std::exception("Unknown solution status");
		}

	}

	auto lam_walk_unknown_all = [](
		const Problem& prob, unsigned int numTries, unsigned int numSteps) {

		if (DEBUG) std::cout << "Random WalkSAT\n";
		auto solver_rand = WalkSolver(numTries, numSteps);
		solver_rand.set_problem(prob);
		solver_rand.create_chooser<RandWalkNodeChooser>();
		auto pair_rand = solver_rand.solve();
		auto sat_rand = check_solution(prob, pair_rand);

		if (DEBUG) std::cout << "GSAT WalkSAT\n";
		auto solver_gsat = WalkSolver(numTries, numSteps);
		solver_gsat.set_problem(prob);
		solver_gsat.create_chooser<GSATNodeChooser>();
		auto pair_gsat = solver_gsat.solve();
		auto sat_gsat = check_solution(prob, pair_gsat);

		if (DEBUG) std::cout << "UnsatClauseMC WalkSAT\n";
		auto solver_mc = WalkSolver(numTries, numSteps);
		solver_mc.set_problem(prob);
		solver_mc.create_chooser<UnsatClauseMCNodeChooser>();
		auto pair_mc = solver_mc.solve();
		auto sat_mc = check_solution(prob, pair_mc);

	};

}

TEST_CASE("WalkSAT", "[sat]") {

	SECTION("Random problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_walk_unknown_all(prob, 5, 20);

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Barabasi-Albert problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::barabasi_albert_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_walk_unknown_all(prob, 5, 20);

		};

		REQUIRE(rc::check(lam));

	}

	SECTION("Watts-Strogatz problems give correct assignments if solvable, and node choice doesn't matter.") {

		auto lam = []() {

			auto prob = *rc::watts_strogatz_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_walk_unknown_all(prob, 5, 20);

		};

		REQUIRE(rc::check(lam));

	}

}
