#include <iostream>
#include <chrono>
#include "alphali/util/math.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



void print_random_problem() {

	using namespace coopplot;

	// Now print a random problem to file
	auto prob = random_problem();

	auto g = prob.get_graph();
	auto p = prob.get_properties();

	using x_type = double;
	using y_type = double;
	range_data<x_type> x_range = { 0.0, 1.0 };
	range_data<y_type> y_range = { -1.0, 1.0 };

	auto num = 20;
	auto diff_x = 2 * alphali::pi / (num - 1);
	auto vec_y = std::vector<double>();
	for (auto i = 0; i < num; ++i) {
		auto x = i*diff_x;
		vec_y.push_back(std::sin(x));
	}
	auto data = sat_xy_data<x_type, y_type>(std::make_pair(diff_x, vec_y));

	print_sat_xy(x_range, y_range, data);

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
