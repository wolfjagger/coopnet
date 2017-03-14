#include "graph_sat.h"
#include "alphali/util/math.h"
#include "alphali/util/timer.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



namespace coopplot {

	void graph_satisfiability(const std::string& foldername) {

		using namespace coopplot;

		auto num_nodes = 100;
		auto start_ratio = 3.5;
		auto end_ratio = 5.5;
		auto num_ratios = 21;
		auto num_average = 20;
		auto node_choice_mode
			= coopnet::DPLLNodeChoiceMode::MostTotClauses;


		auto timer = alphali::timer();


		auto data = create_sat_data(
			num_nodes,
			start_ratio, end_ratio,
			num_ratios, num_average,
			node_choice_mode);


		timer.stop();
		timer.output("create_sat_data");


		std::string title = "Satisfiability with ";
		title += std::to_string(num_nodes);
		title += " nodes";

		RangeData<double> x_range = { 2.0, 6.0 };
		RangeData<double> y_range = { 0.0, 1.0 };


		save_and_plot_sat_xy(x_range, y_range, data,
			foldername, "satisfiability", "gnu_sat",
			title);

	}



	void graph_multi_satisfiability(const std::string& foldername) {

		using namespace coopplot;

		auto start_num_nodes = 20;
		auto end_num_nodes = 160;
		auto num_plots = 8;
		auto start_ratio = 3.5;
		auto end_ratio = 5.5;
		auto num_ratios = 21;
		auto num_average = 100;
		auto node_choice_mode
			= coopnet::DPLLNodeChoiceMode::MostTotClauses;


		auto timer = alphali::timer();


		auto data = create_multiple_sat_data(
			start_num_nodes, end_num_nodes, num_plots,
			start_ratio, end_ratio,
			num_ratios, num_average,
			node_choice_mode);


		timer.stop();
		timer.output("create_multiple_sat_data");


		std::string title = "Multiple num node satisfiability";
		auto plot_titles = std::vector<std::string>();
		auto diff_num_nodes
			= (end_num_nodes - start_num_nodes) / (num_plots - 1);
		for (auto num_nodes = start_num_nodes;
			num_nodes <= end_num_nodes; num_nodes += diff_num_nodes) {
			std::string str;
			str += "num nodes \\= ";
			str += std::to_string(num_nodes);
			plot_titles.push_back(std::move(str));
		}

		RangeData<double> x_range = { 3.0, 6.0 };
		RangeData<double> y_range = { 0.0, 1.0 };


		save_and_plot_sat_xy(x_range, y_range, data,
			foldername, "multi_satisfiability", "gnu_multi_sat",
			title, plot_titles);

	}

}
