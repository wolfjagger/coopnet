#include <istream>
#include <iostream>
#include "graph/test_graph.h"
#include "gephi/output.h"



int main(int, char*[]) {

	auto g = jj_graph::test_graph();
	auto dp = jj_graph::test_props(g);

	jj_gephi::print_graph(g, dp);

	std::cout << "Enter to exit.";
	std::cin.get();

	return 0;

}
