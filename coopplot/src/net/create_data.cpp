#include "create_data.h"
#include "coopnet/sat/problem/problem_factory.h"



namespace coopplot {

	sat_xy_data<double, double> create_sat_data(
		int num_pts, double start_x, double end_x) {

		auto diff_x = (end_x - start_x) / (num_pts - 1);

		auto vec_y = std::vector<double>();
		for (auto i = 0; i < num_pts; ++i) {
			auto x = start_x + i*diff_x;
			vec_y.push_back(std::sin(x));
		}

		return sat_xy_data<double, double>(
			std::make_pair(
				std::array<double, 2>{start_x, diff_x}, vec_y));

		//return sat::generate_random_3sat_problem(28, 100);

	}
	
}
