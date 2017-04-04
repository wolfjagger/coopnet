#pragma once

#include <memory>
#include "solution.h"



namespace coopnet {

	class Problem;

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