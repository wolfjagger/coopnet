#pragma once

#include <memory>
#include <utility>



namespace sat {

	struct assignment;
	class problem;

	enum class solution_status {
		Satisfied, Unsatisfiable, Undetermined
	};


	// Interface for SAT solvers
	class solver {

	public:

		using solve_return = std::pair<solution_status, std::shared_ptr<assignment>>;

	public:

		virtual solve_return solve(const problem& prob) = 0;

	};



	class complete_solver : public solver {

	public:

		solve_return solve(const problem& prob) override;

	protected:

		virtual solve_return do_solve(const problem& prob) = 0;

	};



	class incomplete_solver : public solver {

	public:

		solve_return solve(const problem& prob) override;

	protected:
		
		virtual solve_return try_single_solve(const problem& prob) = 0;

		virtual unsigned int retry_count() const = 0;

	};

}