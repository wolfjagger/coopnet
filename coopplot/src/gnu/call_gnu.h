#pragma once

#include <array>
#include <vector>
#include "boost/variant.hpp"
#include "plot_creator.h"



namespace coopplot {

	template<typename x_type = double, typename y_type = double>
	void save_and_plot_sat_xy(
		range_data<x_type> x_range, range_data<y_type> y_range,
		xy_data<x_type, y_type> data,
		std::string filename) {

		auto plot_maker
			= plot_creator<x_type, y_type>(
				x_range, y_range);
		
		plot_maker.create_dat_file(std::move(filename), data);

		auto gnu_str = plot_maker.gnuplot_str();

		std::cout << "Plotting:" << std::endl;
		std::cout << gnu_str << std::endl;

		std::system(gnu_str.c_str());

	}

}
