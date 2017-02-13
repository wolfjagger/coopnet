#pragma once

#include <vector>
#include "boost/variant.hpp"



namespace coopplot {

	template<class data_type>
	using range_data = std::array<data_type, 2>;

	template<typename x_type, typename y_type>
	using fixed_x = std::pair<range_data<x_type>, std::vector<y_type>>;

	template<typename x_type, typename y_type>
	using varied_x = std::vector<std::pair<x_type, y_type>>;



	template<typename x_type, typename y_type>
	struct sat_xy_data {

		boost::variant<
			fixed_x<x_type, y_type>,
			varied_x<x_type, y_type>> payload;

		template<typename specific_type>
		sat_xy_data(specific_type init_payload) :
			payload(init_payload) {

		}

	};

}
