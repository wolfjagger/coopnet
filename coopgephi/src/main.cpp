#include <iostream>
#include <chrono>
#include "gephi/call_gephi.h"
#include "net/create_data.h"



void print_random_problem() {

	// Now print a random problem to file
	auto prob = coopgephi::random_problem(280, 1000);

	auto g = prob.get_graph();
	auto p = prob.get_properties();

	coopgephi::print_graph(g, p);

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
