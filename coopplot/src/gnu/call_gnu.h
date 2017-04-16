#pragma once

#include <array>
#include <sstream>
#include <vector>
#include "boost/variant.hpp"
#include "plot_creator.h"



namespace coopplot {

	template<typename XType = double, typename YType = double>
	void save_and_plot_xy(
		RangeData<XType> x_range, RangeData<YType> y_range,
		XYData<XType, YType> data,
		const std::string& foldername,
		const std::string& datname,
		const std::string& gnuname,
		std::string title = std::string(),
		std::vector<std::string> titles = std::vector<std::string>(),
		bool withError = false) {

		//TODO: create folder

		auto gnudat_factory
			= GNUDatStringFactory(data);
		auto filename = datname + ".dat";
		auto datfile = PlotFile(foldername, filename, gnudat_factory);

		auto num_y_cols = gnudat_factory.get_num_y_cols();

		auto gnuscript_factory
			= GNUScriptStringFactory(
				datfile.full_path(), x_range, y_range, num_y_cols, withError);

		if (!title.empty()) gnuscript_factory.set_title(title);
		if (!titles.empty()) gnuscript_factory.set_plot_titles(titles);

		auto gnuscriptname = gnuname + ".p";
		auto gnufile = PlotFile(foldername, gnuscriptname, gnuscript_factory);

		auto system_str
			= gnuscript_factory.system_str(gnufile.full_path());
		std::cout << "Plotting:" << std::endl;
		std::cout << system_str << std::endl;
		std::system(system_str.c_str());

	}

}
