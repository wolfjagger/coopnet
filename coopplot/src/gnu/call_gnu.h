#pragma once

#include <array>
#include <vector>
#include "boost/variant.hpp"



namespace coopplot {

	template<class data_type> using range_data = std::array<data_type, 2>;

	template<typename x_type, typename y_type>
	struct sat_xy_data {

		using fixed_x = std::pair<x_type, std::vector<y_type>>;
		using varied_x = std::vector<std::pair<x_type, y_type>>;

		boost::variant<fixed_x, varied_x> payload;

		template<typename specific_type>
		sat_xy_data(specific_type init_payload) :
			payload(init_payload) {

		}

	};

	template<typename x_type = double, typename y_type = double>
	void print_sat_xy(
		range_data<x_type> x_range, range_data<y_type> y_range,
		sat_xy_data<x_type, y_type> data) {

		auto gnu_string = std::string("gnuplot -e \"plot sin(x); pause 5\"");
		std::system(gnu_string.c_str());

	}

}
