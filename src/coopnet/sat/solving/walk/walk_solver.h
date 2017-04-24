#pragma once

#include <stack>
#include "coopnet/sat/solving/solver.h"
#include "coopnet/graph/base/graph.h"
#include "walk_node_chooser.h"



namespace coopnet {

	class WalkFormula;

	class WalkSolver : public IncompleteSolver {

	private:

		static constexpr bool DEBUG = false;

		std::unique_ptr<WalkFormula> formula;

		std::unique_ptr<WalkNodeChooser> nodeChooser;

		unsigned int retryCount;
		size_t maxNumSteps;

	public:

		WalkSolver(
			unsigned int retriesUntilFail,
			size_t numStepsToRetry);

		WalkSolver(const WalkSolver& other) = delete;
		WalkSolver& operator=(const WalkSolver& other) = delete;

		WalkSolver(WalkSolver&& other) = default;
		WalkSolver& operator=(WalkSolver&& other) = default;

		~WalkSolver() override;

		
		
		void set_problem(const Problem& prob) override;

		template<typename ChooserType, typename... Args>
		void create_chooser(Args&&... args) {

			if (!formula) throw std::exception("Formula not set.");

			if (DEBUG) std::cout << "Create node chooser\n";

			nodeChooser = std::make_unique<ChooserType>(
				*formula, std::forward<Args>(args)...);

		}


	
	protected:
		
		Solution try_single_solve() override;

		unsigned int retry_count() const override;

	private:

		size_t find_assignment();

	};

}
