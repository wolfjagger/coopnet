#pragma once

#include <stack>
#include "coopnet/sat/solving/solver.h"
#include "coopnet/graph/base/graph.h"
#include "walk_node_chooser.h"



namespace coopnet {

	class WalkFormula;

	class WalkSolver : public IncompleteSolver {

	private:

		std::unique_ptr<WalkFormula> formula;

		std::unique_ptr<WalkNodeChooser> nodeChooser;

		unsigned int retryCount;
		unsigned int maxNumSteps;

	public:

		// Set this to change how node is chosen
		WalkNodeChoiceMode nodeChoiceMode
			= WalkNodeChoiceMode::Random;

	public:

		WalkSolver(unsigned int triesUntilFail);
		WalkSolver(unsigned int triesUntilFail, WalkNodeChoiceMode mode);

		WalkSolver(const WalkSolver& other) = delete;
		WalkSolver& operator=(const WalkSolver& other) = delete;

		WalkSolver(WalkSolver&& other) = default;
		WalkSolver& operator=(WalkSolver&& other) = default;

		~WalkSolver();
	
	protected:
		
		Solution try_single_solve(const Problem& prob) override;

		unsigned int retry_count() const override;

	private:

		void find_assignment();

	};

}
