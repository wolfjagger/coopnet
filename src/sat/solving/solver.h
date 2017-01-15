#pragma once

#include <memory>
#include <utility>

struct assignment;
class problem;


namespace sat {

	enum class solution_status {
		Satisfied, Unsatisfiable, Undetermined
	};


	// Interface for SAT solvers
	class solver {

	public:

		using solve_return = std::pair<solution_status, std::shared_ptr<assignment>>;
	
	public:

		virtual solve_return solve(const problem& prob) const = 0;

	};


	class complete_solver : solver {
		
	};


	class incomplete_solver : solver {

	public:

		solve_return solve(const problem& prob) const override {
			
			for(unsigned int i=0; i<retry_count(); ++i) {
				auto possible_solution = try_solve(prob);
				if (possible_solution.first != solution_status::Undetermined) {
					return possible_solution;
				}
			}

			return std::make_pair(solution_status::Undetermined, nullptr);

		}

	protected:
		
		virtual solve_return try_solve(const problem& prob) const = 0;

		virtual unsigned int retry_count() const = 0;

	};

}