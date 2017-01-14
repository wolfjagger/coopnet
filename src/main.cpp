#include <istream>
#include <iostream>
#include "graph/testing.h"
#include "gephi/output.h"
#include "sat/component/assignment.h"



int main(int, char*[]) {

	// First, examine all true literals problem.
	auto prob_solvable = testing::all_true_problem();

	// Should be satisfied by an assignment of all true
	std::map<sat::vertex_descriptor, bool> map_assign1;
	for(auto& pair : prob_solvable.get_map_node_to_vert()) {
		map_assign1.emplace(pair.second, true);
	}
	auto assign1 = std::make_shared<sat::assignment>();
	assign1->data = std::move(map_assign1);

	if (prob_solvable.is_satisfied_by(assign1)) {
		std::cout << "Good, problem is satisfied when it should be.\n";
	} else {
		std::cout << "Bad, problem is not satisfied when it should be.\n";
	}

	// Should be unsatisfied by an assignment of all false
	std::map<sat::vertex_descriptor, bool> map_assign2;
	for(auto& pair : prob_solvable.get_map_node_to_vert()) {
		map_assign2.emplace(pair.second, false);
	}
	auto assign2 = std::make_shared<sat::assignment>();
	assign2->data = std::move(map_assign2);

	if (!prob_solvable.is_satisfied_by(assign2)) {
		std::cout << "Good, problem is unsatisfied when it should be.\n";
	} else {
		std::cout << "Bad, problem is not unsatisfied when it should be.\n";
	}


	// Now print a random problem to file
	auto prob = testing::random_problem();

	auto g = prob.get_graph();
	auto p = prob.get_properties();

	jj_gephi::print_graph(g, p);

	std::cout << "Enter to exit.";
	std::cin.get();

	return 0;

}
