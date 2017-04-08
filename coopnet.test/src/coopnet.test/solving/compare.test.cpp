#include <numeric>
#include "catch.hpp"
#include "coopnet/sat/solving/dpll/dpll_solver.h"
#include "coopnet/sat/solving/walk/walk_solver.h"
#include "coopnet.test/prob.gen.h"
#include "coopnet.test/rc_printing.h"

using namespace coopnet;



namespace {

	auto lam_compare_dpll_with_walk = [](const Problem& prob) {

		auto dpll_solver = DPLLSolver(DPLLNodeChoiceMode::MostTotClauses);
		auto dpll_pair = dpll_solver.solve(prob);
		auto walk_solver = WalkSolver(5, 100, WalkNodeChoiceMode::GSAT);
		auto walk_pair = walk_solver.solve(prob);

		switch (walk_pair.status) {
		case SolutionStatus::Satisfied:
			RC_ASSERT(dpll_pair.status == SolutionStatus::Satisfied);
			RC_ASSERT(prob.is_satisfied_by(walk_pair.assignment));
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

	};

}
	
TEST_CASE("Compare solvers", "[sat]") {

	SECTION("WalkSAT reaches same conclusions as DPLL.") {

		auto lam = []() {

			auto prob = *rc::random_prob_gen(
				std::make_pair(3, 10),
				std::make_pair(1, 50));

			lam_compare_dpll_with_walk(prob);

		};

		REQUIRE(rc::check(lam));

	}

}
