#pragma once

#include <memory>
#include "node_choice.h"



namespace coopnet {

	struct Assignment;
	class Problem;

	enum class SolutionStatus {
		Satisfied, Unsatisfiable, Undetermined
	};


	// Interface for SAT solvers
	class Solver {

	public:

		using SolveReturn = std::pair<SolutionStatus, std::shared_ptr<Assignment>>;

	public:

		Solver() = default;

		Solver(const Solver& other) = delete;
		Solver& operator=(const Solver& other) = delete;
		Solver(Solver&& other) = default;
		Solver& operator=(Solver&& other) = default;

		virtual SolveReturn solve(const Problem& prob) = 0;

	};



	class CompleteSolver : public Solver {

	public:

		SolveReturn solve(const Problem& prob) override;

	protected:

		virtual SolveReturn do_solve(const Problem& prob) = 0;

	};



	class IncompleteSolver : public Solver {

	public:

		SolveReturn solve(const Problem& prob) override;

	protected:
		
		virtual SolveReturn try_single_solve(const Problem& prob) = 0;

		virtual unsigned int retry_count() const = 0;

	};

}