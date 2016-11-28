#include <istream>
#include <iostream>
#include "graph/test_graph.h"
#include "gephi/output.h"



int main(int, char*[]) {

	auto graph = JJGraph::TestGraph();
	auto dp = JJGraph::TestProps(graph);

	Gephi::printGraph(graph, dp);

	std::cout << "Enter to exit.";
	std::cin.get();

	return 0;

}
