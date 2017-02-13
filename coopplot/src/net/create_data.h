#pragma once

#include "sat_xy_data.h"



namespace coopplot {

	sat_xy_data<double, double> create_sat_data(
		int num_nodes,
		double start_ratio_clause_node, double end_ratio_clause_node,
		int num_pts, int num_average);

}
