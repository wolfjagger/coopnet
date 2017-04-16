#include "graph_sat.h"
#include "alphali/util/math.h"
#include "alphali/util/timer.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



namespace coopplot {

	void graph_satisfiability(const std::string& foldername, bool withTime) {

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


		std::string titleSat = "Satisfiability with ";
		titleSat += std::to_string(num_nodes);
		titleSat += " nodes";
		std::string titleTime = "Time with ";
		titleTime += std::to_string(num_nodes);
		titleTime += " nodes";
		
		auto plotTitlesTime = std::vector<std::string>();

		RangeData<double> x_range = { 2.0, 6.0 };
		RangeData<double> y_rangeSat = { 0.0, 1.0 };
		RangeData<double> y_rangeTime = { 0.0, 10.0 };


		save_and_plot_xy(x_range, y_rangeSat, data.sat,
			foldername, "satisfiability", "gnu_sat",
			titleSat);

		if(withTime) {
			save_and_plot_xy(x_range, y_rangeTime, data.time,
				foldername, "sat_time", "gnu_time",
				titleTime, plotTitlesTime, true);
		}

	}



	void graph_multi_satisfiability(const std::string& foldername, bool withTime) {

		using namespace coopplot;

		auto start_num_nodes = 20;
		auto end_num_nodes = 120;
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


		std::string titleSat = "Multiple num node satisfiability";
		std::string titleTime = "Multiple num node time elapsed";

		auto plotTitlesSat = std::vector<std::string>();
		auto plotTitlesTime = std::vector<std::string>();
		auto diffNumNodes
			= (end_num_nodes - start_num_nodes) / (num_plots - 1);
		for (auto numNodes = start_num_nodes;
			numNodes <= end_num_nodes; numNodes += diffNumNodes) {
			std::string numStr = std::to_string(numNodes);
			std::string str;
			str = "num nodes \\= ";
			str += numStr;
			plotTitlesSat.push_back(str);
			plotTitlesTime.push_back(str);
		}

		RangeData<double> x_range = { 3.0, 6.0 };
		RangeData<double> y_rangeSat = { 0.0, 1.0 };
		RangeData<double> y_rangeTime = { 0.0, 10.0 };


		save_and_plot_xy(x_range, y_rangeSat, data.sat,
			foldername, "multi_satisfiability", "gnu_multi_sat",
			titleSat, plotTitlesSat, false);

		if (withTime) {
			save_and_plot_xy(x_range, y_rangeTime, data.time,
				foldername, "multi_sat_time", "gnu_multi_time",
				titleSat, plotTitlesTime, true);
		}

	}

}
