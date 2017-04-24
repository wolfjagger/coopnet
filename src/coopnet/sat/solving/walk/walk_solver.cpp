#include "walk_solver.h"
#include <iostream>
#include "boost/logic/tribool.hpp"
#include "coopnet/sat/problem/problem.h"
#include "walk_formula.h"
#include "walk_node_chooser.h"

using namespace coopnet;



namespace {

}



WalkSolver::WalkSolver(
	unsigned int retriesUntilFail,
	size_t numStepsToRetry) :
	formula(nullptr),
	retryCount(retriesUntilFail),
	maxNumSteps(numStepsToRetry) {

}

WalkSolver::~WalkSolver() {

}



void WalkSolver::set_problem(const Problem& prob) {

	if (DEBUG) {
		std::cout << "Solving problem:\n";
		std::cout << prob;
		std::cout << "Construct formula\n";
	}

	formula = std::make_unique<WalkFormula>(prob);
	nodeChooser = nullptr;

}



Solution WalkSolver::try_single_solve() {

	if (!formula || !nodeChooser)
		throw std::exception("Cannot WalkSolve before setting formula and node chooser.");

	if (DEBUG) std::cout << "Set random formula assignment\n";

	auto randAssignment = rand_assignment(
		formula->node_begin(), formula->node_end());
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
		formula = nullptr;
	} else {
		solution = Solution{
			SolutionStatus::Undetermined,
			nullptr,
			0,
			numSteps
		};
		formula->reset();
	}

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

	if (DEBUG) std::cout << formula->create_assignment();

	return stepNum;

}
