#include "graph_sat.h"
#include "alphali/util/math.h"
#include "alphali/util/timer.h"
#include "net/create_data.h"
#include "gnu/call_gnu.h"



namespace coopplot {

	void graph_satisfiability(const std::string& foldername, bool withTime) {

		using namespace coopplot;

		auto numNodes = 100;
		auto startRatio = 3.5;
		auto endRatio = 5.5;
		auto numRatios = 21;
		auto numAvg = 40;
		auto dpllNodeChoiceMode
			= coopnet::DPLLNodeChoiceMode::MostTotClauses;
		auto walkNodeChoiceMode
			= coopnet::WalkNodeChoiceMode::UnsatClauseMC;


		auto timer = alphali::timer();


		auto data = create_sat_data_comparison(
			numNodes,
			startRatio, endRatio,
			numRatios, numAvg,
			dpllNodeChoiceMode,
			walkNodeChoiceMode);


		timer.stop();
		timer.output("create_sat_data");


		std::string titleSat = "Satisfiability with ";
		titleSat += std::to_string(numNodes);
		titleSat += " nodes";
		std::string titleTime = "Time with ";
		titleTime += std::to_string(numNodes);
		titleTime += " nodes";
		
		auto plotTitles = std::vector<std::string>();
		plotTitles.push_back("DPLL");
		plotTitles.push_back("Walk");

		RangeData<double> xRange = { 2.0, 6.0 };
		RangeData<double> yRangeSat = { 0.0, 1.0 };
		RangeData<double> yRangeTime = { 0.0, 1.0 };


		save_and_plot_xy(xRange, yRangeSat, data.sat,
			foldername, "satisfiability", "gnu_sat",
			titleSat, plotTitles, false);

		if(withTime) {
			save_and_plot_xy(xRange, yRangeTime, data.time,
				foldername, "sat_time", "gnu_time",
				titleTime, plotTitles, true);
		}

	}



	void graph_multi_satisfiability(const std::string& foldername, bool withTime) {

		using namespace coopplot;

		auto startNumNodes = 20;
		auto endNumNodes = 200;
		auto numPlots = 10;
		auto startRatio = 3.5;
		auto endRatio = 5.5;
		auto numRatios = 21;
		auto numAvg = 100;
		auto nodeChoiceMode
			= coopnet::DPLLNodeChoiceMode::MostTotClauses;


		auto timer = alphali::timer();


		auto data = create_multiple_sat_data(
			startNumNodes, endNumNodes, numPlots,
			startRatio, endRatio,
			numRatios, numAvg,
			nodeChoiceMode);


		timer.stop();
		timer.output("create_multiple_sat_data");


		std::string titleSat = "Multiple num node satisfiability";
		std::string titleTime = "Multiple num node time elapsed";

		auto plotTitles = std::vector<std::string>();
		auto diffNumNodes
			= (endNumNodes - startNumNodes) / (numPlots - 1);
		for (auto numNodes = startNumNodes;
			numNodes <= endNumNodes; numNodes += diffNumNodes) {
			std::string numStr = std::to_string(numNodes);
			std::string str;
			str = "num nodes \\= ";
			str += numStr;
			plotTitles.push_back(std::string("DPLL") + str);
			plotTitles.push_back(std::string("Walk") + str);
		}

		RangeData<double> xRange = { 3.0, 6.0 };
		RangeData<double> yRangeSat = { 0.0, 1.0 };
		RangeData<double> yRangeTime = { 0.0, 10.0 };


		save_and_plot_xy(xRange, yRangeSat, data.sat,
			foldername, "multi_satisfiability", "gnu_multi_sat",
			titleSat, plotTitles, false);

		if (withTime) {
			save_and_plot_xy(xRange, yRangeTime, data.time,
				foldername, "multi_sat_time", "gnu_multi_time",
				titleSat, plotTitles, true);
		}

	}

}
