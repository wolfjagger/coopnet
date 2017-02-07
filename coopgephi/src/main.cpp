#include <iostream>
#include <chrono>
#include "graph/testing.h"
#include "gephi/output.h"



void print_random_problem() {

	// Now print a random problem to file
	auto prob = testing::random_problem();

	auto g = prob.get_graph();
	auto p = prob.get_properties();

	jj_gephi::print_graph(g, p);

}



int main(int, char*[]) {

	using namespace std::chrono;

	auto clock = system_clock();
	auto beg_time = clock.now();

	print_random_problem();

	auto end_time = clock.now();

	auto time_diff = duration_cast<milliseconds>(end_time - beg_time);
	auto secs = float(time_diff.count()) / 1000;

	std::cout << "Took " << secs << " secs" << std::endl;

	return 0;

}
