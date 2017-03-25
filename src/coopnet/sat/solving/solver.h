#pragma once

#include <ostream>
#include <memory>
#include "node_choice.h"



namespace coopnet {

	struct Assignment;
	class Problem;

	enum class SolutionStatus {
		Satisfied, Partial, Unsatisfied, Undetermined
	};

	inline std::ostream& operator<<(
		std::ostream& os, coopnet::SolutionStatus status);

	struct Solution {
		SolutionStatus status;
		std::shared_ptr<Assignment> assignment;
		unsigned int num_failed;
	};

	inline std::ostream& operator<<(
		std::ostream& os, const Solution& solution);



	// Interface for SAT solvers
	class Solver {

	public:

		Solver() = default;

		Solver(const Solver& other) = delete;
		Solver& operator=(const Solver& other) = delete;
		Solver(Solver&& other) = default;
		Solver& operator=(Solver&& other) = default;

		virtual Solution solve(const Problem& prob) = 0;

	};



	class CompleteSolver : public Solver {

	public:

		Solution solve(const Problem& prob) override;

	protected:

		virtual Solution do_solve(const Problem& prob) = 0;

	};



	class IncompleteSolver : public Solver {

	public:

		Solution solve(const Problem& prob) override;

	protected:
		
		virtual Solution try_single_solve(const Problem& prob) = 0;

		virtual unsigned int retry_count() const = 0;

	};

}