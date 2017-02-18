#pragma once

#include "gnu/xy_data.h"
#include "coopnet/sat/solving/dpll/dpll_node_chooser.h"



namespace coopplot {

	xy_data<double, double> create_sat_data(
		int num_nodes,
		double start_ratio_clause_node, double end_ratio_clause_node,
		int num_ratios, int num_average,
		sat::dpll_node_choice_mode node_choice_mode);

	xy_data<double, double> create_multiple_sat_data(
		int start_num_nodes, int end_num_nodes, int num_plots,
		double start_ratio_clause_node, double end_ratio_clause_node,
		int num_ratios, int num_average,
		sat::dpll_node_choice_mode node_choice_mode);

}
