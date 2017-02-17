#include <chrono>
#include "alphali/util/math.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



void graph_satisfiability(const std::string& foldername) {

	using namespace coopplot;

	auto num_nodes = 70;
	auto start_ratio = 3.5;
	auto end_ratio = 5.5;
	auto num_ratios = 21;
	auto num_average = 20;


	using namespace std::chrono;

	auto clock = system_clock();
	auto beg_time = clock.now();


	auto data = create_sat_data(
		num_nodes,
		start_ratio, end_ratio,
		num_ratios, num_average);


	auto end_time = clock.now();

	auto time_diff = duration_cast<milliseconds>(end_time - beg_time);
	auto secs = float(time_diff.count()) / 1000;

	std::cout << "create_sat_data took " << secs << " secs" << std::endl;


	std::string title = "Satisfiability with ";
	title += std::to_string(num_nodes);
	title += " nodes";

	range_data<double> x_range = { 2.0, 6.0 };
	range_data<double> y_range = { 0.0, 1.0 };


	save_and_plot_sat_xy(x_range, y_range, data,
		foldername, "satisfiability", "gnu_sat",
		title);

}



void graph_multi_satisfiability(const std::string& foldername) {

	using namespace coopplot;

	auto start_num_nodes = 20;
	auto end_num_nodes = 80;
	auto num_plots = 4;
	auto start_ratio = 3.5;
	auto end_ratio = 5.5;
	auto num_ratios = 21;
	auto num_average = 20;


	using namespace std::chrono;

	auto clock = system_clock();
	auto beg_time = clock.now();


	auto data = create_multiple_sat_data(
		start_num_nodes, end_num_nodes, num_plots,
		start_ratio, end_ratio,
		num_ratios, num_average);


	auto end_time = clock.now();

	auto time_diff = duration_cast<milliseconds>(end_time - beg_time);
	auto secs = float(time_diff.count()) / 1000;

	std::cout << "create_multiple_sat_data took "
		<< secs << " secs" << std::endl;


	std::string title = "Multiple num_node satisfiability";
	auto plot_titles = std::vector<std::string>();
	auto diff_num_nodes
		= (end_num_nodes - start_num_nodes) / (num_plots - 1);
	for(auto num_nodes = start_num_nodes;
		num_nodes <= end_num_nodes; num_nodes += diff_num_nodes) {
		std::string str;
		str += "num nodes \\= ";
		str += std::to_string(num_nodes);
		plot_titles.push_back(std::move(str));
	}

	range_data<double> x_range = { 3.0, 6.0 };
	range_data<double> y_range = { 0.0, 1.0 };

	
	save_and_plot_sat_xy(x_range, y_range, data,
		foldername, "multi_satisfiability", "gnu_multi_sat",
		title, plot_titles);

}



int main(int, char*[]) {

	auto foldername = std::string("tmp_folder");

	graph_satisfiability(foldername);

	graph_multi_satisfiability(foldername);

	return 0;

}
