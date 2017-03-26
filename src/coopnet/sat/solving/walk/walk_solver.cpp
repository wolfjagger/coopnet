#include "walk_solver.h"
#include <iostream>
#include "boost/logic/tribool.hpp"
#include "coopnet/sat/problem/problem.h"
#include "walk_formula.h"
#include "walk_node_chooser.h"

using namespace coopnet;



namespace {

	constexpr bool DEBUG = false;

	void DEBUG_print_assignment(const WalkFormula& formula) {
		if (DEBUG) {
			auto print_pred =
				[](IncompleteAssignment::value_type pair) {
				if (pair.second) {
					std::cout << "T";
				} else if (!pair.second) {
					std::cout << "F";
				} else if (boost::logic::indeterminate(pair.second)) {
					std::cout << "I";
				} else {
					std::cout << "O";
				}
			};
			auto& assign = formula.create_incomplete_assignment();
			std::for_each(assign.cbegin(), assign.cend(), print_pred);
			std::cout << "\n";
		}
	}

}



WalkSolver::WalkSolver(WalkNodeChoiceMode mode, unsigned int triesUntilFail) :
	nodeChooser(WalkNodeChooser::create(mode)),
	retryCount(triesUntilFail) {

}

WalkSolver::~WalkSolver() {

}



Solution WalkSolver::try_single_solve(const Problem& prob) {

	if (DEBUG) {
		std::cout << "Solving problem:\n";
		std::cout << prob;
	}

	formula = std::make_unique<WalkFormula>(prob);

	auto nodeList = prob.generate_node_list();
	auto randAssignment = rand_assignment(nodeList.begin(), nodeList.end());
	formula->set_assignment(randAssignment);

	find_assignment();

	Solution solution;
	if (formula->is_SAT()) {
		auto assign = Assignment(formula->create_assignment());
		solution = Solution{
			SolutionStatus::Satisfied,
			std::make_shared<coopnet::Assignment>(std::move(assign)),
			0
		};
	} else {
		solution = Solution{
			SolutionStatus::Undetermined,
			nullptr,
			0
		};
	}

	formula = nullptr;
	return solution;

}

unsigned int WalkSolver::retry_count() const {
	return retryCount;
}



void WalkSolver::find_assignment() {

	unsigned int numSteps = 0;
	while (numSteps < maxNumSteps) {

		auto nodeToFlip = nodeChooser->choose(*formula);

		formula->flip_node(nodeToFlip);
		if (formula->is_SAT()) break;
		
	}

	DEBUG_print_assignment(*formula);

}
