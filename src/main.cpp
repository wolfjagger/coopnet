#include <istream>
#include <iostream>
#include "graph/testing.h"
#include "gephi/output.h"
#include "sat/component/assignment.h"



void print_random_problem() {

	// Now print a random problem to file
	auto prob = testing::random_problem();

	auto g = prob.get_graph();
	auto p = prob.get_properties();

	jj_gephi::print_graph(g, p);

	std::cout << "Enter to exit.";
	std::cin.get();

}

void check_problem(const sat::problem& prob) {

	// Should be satisfied by an assignment of all true
	std::map<sat::vertex_descriptor, bool> map_assign1;
	for(auto& pair : prob.get_map_node_to_vert()) {
		map_assign1.emplace(pair.second, true);
	}
	auto assign1 = std::make_shared<sat::assignment>();
	assign1->data = std::move(map_assign1);

	if (prob.is_satisfied_by(assign1)) {
		std::cout << "Good, problem is satisfied when it should be.\n";
	} else {
		std::cout << "Bad, problem is not satisfied when it should be.\n";
	}

	// Should be unsatisfied by an assignment of all false
	std::map<sat::vertex_descriptor, bool> map_assign2;
	for(auto& pair : prob.get_map_node_to_vert()) {
		map_assign2.emplace(pair.second, false);
	}
	auto assign2 = std::make_shared<sat::assignment>();
	assign2->data = std::move(map_assign2);

	if (!prob.is_satisfied_by(assign2)) {
		std::cout << "Good, problem is unsatisfied when it should be.\n";
	} else {
		std::cout << "Bad, problem is not unsatisfied when it should be.\n";
	}

}

void check_solvable_problem() {

	std::cout << "Connected all-true problem\n";

	// First, examine all true literals problem.
	auto prob_solvable = testing::all_true_problem();

	check_problem(prob_solvable);

}

void check_solvable_disconnected_problem() {

	std::cout << "Disconnected all-true problem\n";

	// Examine all true literals problem with two components.
	auto prob_solvable = testing::all_true_two_problems();

	check_problem(prob_solvable);

}



int main(int, char*[]) {

	check_solvable_problem();

	check_solvable_disconnected_problem();

	print_random_problem();

	return 0;

}
