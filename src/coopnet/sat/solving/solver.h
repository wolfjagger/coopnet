#pragma once

#include <memory>



namespace sat {

	struct assignment;
	class problem;
	class node_chooser;

	enum class solution_status {
		Satisfied, Unsatisfiable, Undetermined
	};


	// Interface for SAT solvers
	class solver {

	protected:

		std::unique_ptr<node_chooser> n_chooser;

	public:

		using solve_return = std::pair<solution_status, std::shared_ptr<assignment>>;

	public:

		solver(std::unique_ptr<node_chooser> chooser);

		solver(const solver& other) = delete;
		solver& operator=(const solver& other) = delete;
		solver(solver&& other) = default;
		solver& operator=(solver&& other) = default;

		~solver();

		virtual solve_return solve(const problem& prob) = 0;

	};



	class complete_solver : public solver {

	public:

		complete_solver(std::unique_ptr<node_chooser> chooser);

		complete_solver(complete_solver&& other) = default;
		complete_solver& operator=(complete_solver&& other) = default;

		~complete_solver();

		solve_return solve(const problem& prob) override;

	protected:

		virtual solve_return do_solve(const problem& prob) = 0;

	};



	class incomplete_solver : public solver {

	public:

		incomplete_solver(std::unique_ptr<node_chooser> chooser);

		incomplete_solver(incomplete_solver&& other) = default;
		incomplete_solver& operator=(incomplete_solver&& other) = default;

		~incomplete_solver();

		solve_return solve(const problem& prob) override;

	protected:
		
		virtual solve_return try_single_solve(const problem& prob) = 0;

		virtual unsigned int retry_count() const = 0;

	};

}