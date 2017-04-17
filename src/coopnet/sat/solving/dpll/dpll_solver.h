#pragma once

#include <stack>
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/solving/solver.h"
#include "coopnet/graph/base/graph.h"
#include "dpll_node_chooser.h"



namespace coopnet {

	class DPLLFormula;

	class DPLLSolver : public CompleteSolver {

	private:

		std::unique_ptr<DPLLFormula> formula;

		std::unique_ptr<DPLLNodeChooser> nodeChooser;

		// Choice: node, sgn, first time chosen (out of two)
		std::stack<DPLLNodeChoiceBranch> decisions;

	public:

		// Set this to change how node is chosen
		DPLLNodeChoiceMode nodeChoiceMode;

	public:

		DPLLSolver(
			DPLLNodeChoiceMode mode = DPLLNodeChoiceMode::MostTotClauses);

		DPLLSolver(const DPLLSolver& other) = delete;
		DPLLSolver& operator=(const DPLLSolver& other) = delete;

		DPLLSolver(DPLLSolver&& other) = default;
		DPLLSolver& operator=(DPLLSolver&& other) = default;

		~DPLLSolver() override;
	
	protected:
		
		virtual Solution do_solve(const Problem& prob) override;

	private:

		void find_assignment();

		bool change_last_free_choice();
		void reduce_with_selection(DPLLNodeChoiceBranch decision);

	};

}
