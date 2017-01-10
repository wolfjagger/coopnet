#include <istream>
#include <iostream>
#include "graph/testing.h"
#include "gephi/output.h"



int main(int, char*[]) {

	auto prob = testing::test_problem();
	auto g = prob.get_graph();
	auto p = prob.get_props();

	jj_gephi::print_graph(g, p);

	std::cout << "Enter to exit.";
	std::cin.get();

	return 0;

}
