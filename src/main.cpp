#include <istream>
#include <iostream>
#include "graph/testing.h"
#include "gephi/output.h"
#include "sat/assignment/assignment.h"
#include "sat/solving/dpll/dpll_solver.h"



void check_problem(const sat::problem& prob, bool assignment_sgn) {

	// Should be satisfied by an assignment of all true
	std::map<sat::vertex_descriptor, bool> map_assign1;
	for(auto& pair : prob.get_map_node_to_vert()) {
		map_assign1.emplace(pair.second, assignment_sgn);
	}
	auto assign1 = std::make_shared<sat::assignment>();
	assign1->data = std::move(map_assign1);

	if (prob.is_satisfied_by(assign1)) {
		std::cout << "Good, problem is satisfied when it should be.\n";
	} else {
		std::cout << "Bad, problem is not satisfied when it should be.\n";
	}

	// Should (probably) be unsatisfied by an assignment of all false
	std::map<sat::vertex_descriptor, bool> map_assign2;
	for(auto& pair : prob.get_map_node_to_vert()) {
		map_assign2.emplace(pair.second, !assignment_sgn);
	}
	auto assign2 = std::make_shared<sat::assignment>();
	assign2->data = std::move(map_assign2);

	if (!prob.is_satisfied_by(assign2)) {
		std::cout << "Good, problem is unsatisfied when it should be.\n";
	} else {
		std::cout << "Bad, problem is not unsatisfied when it should be.\n";
	}

	// Try the dpll_solver
	auto solver = sat::dpll_solver();

	auto pair = solver.solve(prob);

	switch (pair.first) {
	case sat::solution_status::Satisfied:
		std::cout << "Satisfied!\n";
		break;
	case sat::solution_status::Unsatisfiable:
		std::cout << "Unsatisfiable!\n";
		break;
	case sat::solution_status::Undetermined:
		std::cout << "Undetermined!\n";
		break;
	}

	auto assign = pair.second;
	if (assign) {
		std::cout << "Assignment found.\n";
	} else {
		std::cout << "Assignment not found.\n";
	}

}



void check_solvable_problem() {

	std::cout << "Connected all-true problem\n";

	// Examine all true literals problem.
	auto prob_solvable = testing::solvable_problem(true);

	check_problem(prob_solvable, true);


	std::cout << "Connected all-false problem\n";

	// Examine all false literals problem.
	prob_solvable = testing::solvable_problem(false);

	check_problem(prob_solvable, false);

}

void check_solvable_disconnected_problem() {

	std::cout << "Disconnected all-true problem\n";

	// Examine all true literals problem with two components.
	auto prob_solvable = testing::two_solvable_problems(true);

	check_problem(prob_solvable, true);


	std::cout << "Disconnected all-false problem\n";

	// Examine all true literals problem with two components.
	prob_solvable = testing::two_solvable_problems(false);

	check_problem(prob_solvable, false);

}



void check_solver_gives_correct_assignments(
	int num_random_probs) {

	std::cout << "\n";

	auto solver = sat::dpll_solver();

	for(auto i=0; i<num_random_probs; ++i) {

		auto prob = testing::random_problem();
		
		auto pair = solver.solve(prob);
		switch(pair.first) {
		case sat::solution_status::Satisfied:
			std::cout << "Problem satisfiable.\n";
			if(prob.is_satisfied_by(pair.second)) {
				std::cout << "Solution works.\n";
			} else {
				std::cout << "Solution fails!!!\n";
			}
			break;
		case sat::solution_status::Unsatisfiable:
			std::cout << "Problem unsatisfiable.\n";
			break;
		case sat::solution_status::Undetermined:
			std::cout << "Satisfiability undetermined!!!";
			break;
		}

	}

}

void print_random_problem() {

	// Now print a random problem to file
	auto prob = testing::random_problem();

	auto g = prob.get_graph();
	auto p = prob.get_properties();

	jj_gephi::print_graph(g, p);

}



int main(int, char*[]) {

	check_solvable_problem();

	check_solvable_disconnected_problem();

	check_solver_gives_correct_assignments(10);

	print_random_problem();

	std::cout << "Enter to exit.\n";
	std::cin.get();

	return 0;

}
