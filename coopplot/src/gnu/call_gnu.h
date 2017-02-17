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
		std::string foldername, std::string datname, std::string gnuname) {

		//TODO: create folder

		auto gnudat_factory
			= gnudat_string_factory<x_type, y_type>(
				data);
		auto datfile = plotfile(foldername, datname, gnudat_factory);

		auto gnuscript_factory
			= gnuscript_string_factory<x_type, y_type>(
				datfile.full_path(), x_range, y_range,
				gnudat_factory.get_num_y_cols());
		auto scriptfile = plotfile(foldername, gnuname, gnuscript_factory);

		auto script_string = scriptfile.get_file_str();

		std::cout << "Plotting:" << std::endl;
		std::cout << script_string << std::endl;

		std::system(script_string.c_str());

	}

}
