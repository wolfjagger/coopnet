#pragma once

#include "sat_xy_data.h"



namespace coopplot {

	sat_xy_data<double, double> create_sat_data(
		int num_pts, double start_x, double end_x);

}
