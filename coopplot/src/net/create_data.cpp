#include "create_data.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/problem/problem_factory.h"
#include "coopnet/sat/solving/dpll/dpll_solver.h"



namespace coopplot {

	sat_xy_data<double, double> create_sat_data(
		int num_nodes,
		double start_ratio_clause_node, double end_ratio_clause_node,
		int num_pts, int num_average) {

		auto diff_ratio
			= (end_ratio_clause_node - start_ratio_clause_node) / (num_pts - 1);

		auto solver = sat::dpll_solver();

		auto vec_y = std::vector<double>();
		for (auto i = 0; i < num_pts; ++i) {

			auto ratio = start_ratio_clause_node + i*diff_ratio;
			auto num_clauses = unsigned int(num_nodes * std::ceil(ratio));
			auto num_satisfiable = 0;

			std::cout << "Ratio: " << ratio << std::endl;

			for(auto j=0; j < num_average; ++j) {

				auto problem
					= sat::generate_random_3sat_problem(num_nodes, num_clauses);
				auto solution_pair = solver.solve(problem);
				if (solution_pair.first == sat::solution_status::Satisfied)
					++num_satisfiable;

			}

			auto frac_satisfiable = double(num_satisfiable) / num_average;

			vec_y.push_back(frac_satisfiable);

		}

		auto x_domain = std::array<double, 2>{
			start_ratio_clause_node, diff_ratio };

		return sat_xy_data<double, double>(
			std::make_pair(x_domain, vec_y));

	}
	
}
