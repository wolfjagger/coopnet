#include <chrono>
#include "alphali/util/math.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



void print_random_problem() {

	using namespace coopplot;

	//TODO: Replace with calculation to get sat_data from coopnet satisfiability.
	auto prob = random_problem();

	using x_type = double;
	using y_type = double;
	range_data<x_type> x_range = { 0.0, 7.0 };
	range_data<y_type> y_range = { -1.0, 1.0 };

	auto num = 20;
	auto start_x = 0.0;
	auto diff_x = 2 * alphali::pi / (num - 1);
	auto vec_y = std::vector<double>();
	for (auto i = 0; i < num; ++i) {
		auto x = start_x + i*diff_x;
		vec_y.push_back(std::sin(x));
	}
	auto data = sat_xy_data<x_type, y_type>(
		std::make_pair(
			std::array<double, 2>{start_x, diff_x}, vec_y));

	save_and_plot_sat_xy(x_range, y_range, data, "tmp_sat.dat");

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
