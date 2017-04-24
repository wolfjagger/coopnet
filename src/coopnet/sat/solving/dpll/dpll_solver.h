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

		static constexpr bool DEBUG = false;

		std::unique_ptr<DPLLFormula> formula;

		std::unique_ptr<DPLLNodeChooser> nodeChooser;

		// Choice: node, sgn, first time chosen (out of two)
		std::stack<DPLLNodeChoiceBranch> decisions;

	public:

		DPLLSolver();

		DPLLSolver(const DPLLSolver& other) = delete;
		DPLLSolver& operator=(const DPLLSolver& other) = delete;

		DPLLSolver(DPLLSolver&& other);
		DPLLSolver& operator=(DPLLSolver&& other);

		~DPLLSolver() override;



		void set_problem(const Problem& prob) override;

		void set_chooser(std::unique_ptr<DPLLNodeChooser> newChooser);

		template<typename ChooserType, typename... Args>
		void create_chooser(Args&&... args) {

			auto chooser = std::make_unique<ChooserType>(
				std::forward<Args>(args)...);

			set_chooser(std::move(chooser));

		}


	
	protected:
		
		virtual Solution do_solve() override;

	private:

		void find_assignment();

		bool change_last_free_choice();
		void reduce_with_selection(DPLLNodeChoiceBranch decision);

		void DEBUG_print_assignment(const DPLLFormula& formula);

	};

}
