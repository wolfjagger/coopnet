#pragma once

#include <array>
#include <sstream>
#include <vector>
#include "boost/variant.hpp"
#include "plot_creator.h"



namespace coopplot {

	template<typename x_type = double, typename y_type = double>
	void save_and_plot_sat_xy(
		range_data<x_type> x_range, range_data<y_type> y_range,
		xy_data<x_type, y_type> data,
		const std::string& foldername,
		const std::string& datname,
		const std::string& gnuname,
		std::string title = std::string(),
		std::vector<std::string> titles = std::vector<std::string>()) {

		//TODO: create folder

		auto gnudat_factory
			= gnudat_string_factory<x_type, y_type>(data);
		auto filename = datname + ".dat";
		auto datfile = plotfile(foldername, filename, gnudat_factory);

		auto num_y_cols = gnudat_factory.get_num_y_cols();

		auto gnuscript_factory
			= gnuscript_string_factory<x_type, y_type>(
				datfile.full_path(), x_range, y_range, num_y_cols);

		if (!title.empty()) gnuscript_factory.set_title(title);
		if (!titles.empty()) gnuscript_factory.set_plot_titles(titles);

		auto gnuscriptname = gnuname + ".p";
		auto gnufile = plotfile(foldername, gnuscriptname, gnuscript_factory);

		auto system_str
			= gnuscript_factory.system_str(gnufile.full_path());
		std::cout << "Plotting:" << std::endl;
		std::cout << system_str << std::endl;
		std::system(system_str.c_str());

	}

}
