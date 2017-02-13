#include <chrono>
#include "alphali/util/math.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



void graph_satisfiability() {

	using namespace coopplot;

	auto num = 20;
	auto start_x = 0.0;
	auto end_x = 3 * alphali::pi;

	range_data<double> x_range = { std::floor(start_x), std::ceil(end_x) };
	range_data<double> y_range = { -1.0, 1.0 };

	auto data = create_sat_data(num, start_x, end_x);

	save_and_plot_sat_xy(x_range, y_range, data, "satisfiability.dat");

}



int main(int, char*[]) {

	using namespace std::chrono;

	auto clock = system_clock();
	auto beg_time = clock.now();

	graph_satisfiability();

	auto end_time = clock.now();

	auto time_diff = duration_cast<milliseconds>(end_time - beg_time);
	auto secs = float(time_diff.count()) / 1000;

	std::cout << "Took " << secs << " secs" << std::endl;

	return 0;

}
