#include "walk_solver.h"
#include <iostream>
#include "boost/logic/tribool.hpp"
#include "coopnet/sat/problem/problem.h"
#include "walk_formula.h"
#include "walk_node_chooser.h"

using namespace coopnet;



namespace {

	constexpr bool DEBUG = false;

	void DEBUG_print_assignment(const WalkFormula& form) {
		if(DEBUG) {
			std::cout << form.create_assignment();
		}
	}

}



WalkSolver::WalkSolver(
	unsigned int retriesUntilFail,
	size_t numStepsToRetry,
	WalkNodeChoiceMode mode) :
	formula(nullptr),
	retryCount(retriesUntilFail),
	maxNumSteps(numStepsToRetry),
	nodeChoiceMode(mode) {

}

WalkSolver::~WalkSolver() {

}



Solution WalkSolver::try_single_solve(const Problem& prob) {

	if (DEBUG) {
		std::cout << "Solving problem:\n";
		std::cout << prob;
		std::cout << "Construct formula\n";
	}

	formula = std::make_unique<WalkFormula>(prob);
	nodeChooser = WalkNodeChooser::create(*formula, nodeChoiceMode);

	if (DEBUG) std::cout << "Set random formula assignment\n";

	auto& translator = *prob.get_node_vert_translator();
	auto randAssignment = rand_assignment(
		translator.node_begin(), translator.node_end());
	formula->set_assignment(randAssignment);

	if (DEBUG) std::cout << "Find satisfactory assignment\n";

	auto numSteps = find_assignment();

	if (DEBUG) std::cout << "Construct solution\n";

	Solution solution;
	if (formula->is_SAT()) {
		auto assign = Assignment(formula->create_assignment());
		solution = Solution{
			SolutionStatus::Satisfied,
			std::make_shared<coopnet::Assignment>(std::move(assign)),
			0,
			numSteps
		};
	} else {
		solution = Solution{
			SolutionStatus::Undetermined,
			nullptr,
			0,
			numSteps
		};
	}

	formula = nullptr;
	return solution;

}

unsigned int WalkSolver::retry_count() const {
	return retryCount;
}



size_t WalkSolver::find_assignment() {

	size_t stepNum = 0;
	while(stepNum < maxNumSteps) {

		if (DEBUG) std::cout << "Check satisfied\n";
		if (formula->is_SAT()) break;

		++stepNum;

		if (DEBUG) std::cout << "Choose node\n";
		auto nodeToFlip = nodeChooser->choose();

		if (DEBUG) std::cout << "Flip node\n";
		formula->flip_node(nodeToFlip);

	}

	DEBUG_print_assignment(*formula);

	return stepNum;

}
