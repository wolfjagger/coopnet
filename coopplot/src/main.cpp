#include <chrono>
#include "alphali/util/math.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



void graph_satisfiability() {

	using namespace coopplot;

	auto num_nodes = 40;
	auto start_ratio = 2.5;
	auto end_ratio = 5.5;
	auto num_pts = 7;
	auto num_average = 20;

	auto data = create_sat_data(
		num_nodes,
		start_ratio, end_ratio,
		num_pts, num_average);

	range_data<double> x_range = { 2.0, 6.0 };
	range_data<double> y_range = { 0.0, 1.0 };

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
