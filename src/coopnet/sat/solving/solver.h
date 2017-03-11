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

		Solver();

		Solver(const Solver& other) = delete;
		Solver& operator=(const Solver& other) = delete;
		Solver(Solver&& other) = default;
		Solver& operator=(Solver&& other) = default;

		~Solver();

		virtual SolveReturn solve(const Problem& prob) = 0;

	};



	class CompleteSolver : public Solver {

	public:

		CompleteSolver();

		CompleteSolver(CompleteSolver&& other) = default;
		CompleteSolver& operator=(CompleteSolver&& other) = default;

		~CompleteSolver();

		SolveReturn solve(const Problem& prob) override;

	protected:

		virtual SolveReturn do_solve(const Problem& prob) = 0;

	};



	class IncompleteSolver : public Solver {

	public:

		IncompleteSolver();

		IncompleteSolver(IncompleteSolver&& other) = default;
		IncompleteSolver& operator=(IncompleteSolver&& other) = default;

		~IncompleteSolver();

		SolveReturn solve(const Problem& prob) override;

	protected:
		
		virtual SolveReturn try_single_solve(const Problem& prob) = 0;

		virtual unsigned int retry_count() const = 0;

	};

}