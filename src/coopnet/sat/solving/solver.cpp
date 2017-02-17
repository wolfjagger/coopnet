#include "solver.h"
#include "node_chooser.h"

using namespace sat;



solver::solver(std::unique_ptr<node_chooser> chooser) :
	n_chooser(std::move(chooser)) {

}

solver::~solver() {

}



complete_solver::complete_solver(std::unique_ptr<node_chooser> chooser) :
	solver(std::move(chooser)) {

}

complete_solver::~complete_solver() {

}


auto complete_solver::solve(const problem& prob) -> solve_return {
	
	auto solution = do_solve(prob);

	if (solution.first == solution_status::Undetermined)
		throw std::exception("Complete solver has undetermined solution.");

	return solution;

}



incomplete_solver::incomplete_solver(std::unique_ptr<node_chooser> chooser) :
	solver(std::move(chooser)) {

}

incomplete_solver::~incomplete_solver() {

}


auto incomplete_solver::solve(const problem& prob) -> solve_return {
			
	for (unsigned int i = 0; i < retry_count(); ++i) {
		auto possible_solution = try_single_solve(prob);
		if (possible_solution.first != solution_status::Undetermined) {
			return possible_solution;
		}
	}

	return std::make_pair(solution_status::Undetermined, nullptr);

}
