#include "create_data.h"
#include "alphali/util/timer.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/problem/creation/problem_factory.h"
#include "coopnet/sat/solving/dpll/dpll_solver.h"

using namespace coopnet;



namespace {
	constexpr bool PRINT_sub_timing = false;
}



namespace coopplot {

	namespace {

		std::array<double, 2> create_x_domain(
			double start_ratio, double end_ratio, int num_ratios) {

			auto diff_ratio
				= (end_ratio - start_ratio) / (num_ratios - 1);

			return std::array<double, 2>{
				start_ratio, diff_ratio };

		}

		double frac_satisfiable(
			DPLLSolver& solver,
			int num_nodes, int num_clauses, int num_average) {
			
			auto num_satisfiable = 0;

			for (auto j = 0; j < num_average; ++j) {

				if(PRINT_sub_timing) {

					auto timer = alphali::timer();

					auto problem
						= problem_factory::random_3sat_problem(num_nodes, num_clauses);
					auto solution_pair = solver.solve(problem);
					if (solution_pair.status == SolutionStatus::Satisfied)
						++num_satisfiable;

					timer.stop();
					timer.output("Generate/solve");

				} else {

					auto problem
						= problem_factory::random_3sat_problem(num_nodes, num_clauses);
					auto solution_pair = solver.solve(problem);
					if (solution_pair.status == SolutionStatus::Satisfied)
						++num_satisfiable;

				}

			}

			return double(num_satisfiable) / num_average;

		}

	}



	XYData<double, double> create_sat_data(
		int num_nodes,
		double start_ratio_clause_node, double end_ratio_clause_node,
		int num_ratios, int num_average,
		DPLLNodeChoiceMode node_choice_mode) {

		auto x_domain = create_x_domain(
			start_ratio_clause_node, end_ratio_clause_node, num_ratios);

		auto solver = DPLLSolver(node_choice_mode);

		auto vec_y = std::vector<double>();
		vec_y.reserve(num_ratios);
		for (auto i = 0; i < num_ratios; ++i) {

			auto timer = alphali::timer();
				
			auto ratio = start_ratio_clause_node + i*x_domain[1];
			auto num_clauses = unsigned int(std::ceil(num_nodes * ratio));

			std::cout << "Ratio: " << ratio << std::endl;

			vec_y.push_back(
				frac_satisfiable(
					solver, num_nodes, num_clauses, num_average));

			timer.stop();
			timer.output("Set of num_node/num_clause");

		}

		return XYData<double, double>(
			std::make_pair(x_domain, vec_y));

	}


	XYData<double, double> create_multiple_sat_data(
		int start_num_nodes, int end_num_nodes, int num_plots,
		double start_ratio_clause_node, double end_ratio_clause_node,
		int num_ratios, int num_average,
		DPLLNodeChoiceMode node_choice_mode) {

		auto x_domain = create_x_domain(
			start_ratio_clause_node, end_ratio_clause_node, num_ratios);

		auto diff_num_nodes = (end_num_nodes - start_num_nodes) / (num_plots-1);

		auto solver = DPLLSolver(node_choice_mode);

		auto vec_ys = std::vector<std::vector<double>>();
		vec_ys.reserve(num_ratios);
		for (auto i = 0; i < num_ratios; ++i) {

			auto ratio = start_ratio_clause_node + i*x_domain[1];

			std::cout << "Ratio: " << ratio << std::endl;

			auto vec_y = std::vector<double>();
			vec_y.reserve(num_plots);
			for(auto j=0; j < num_plots; ++j) {

				auto timer = alphali::timer();

				auto num_nodes = start_num_nodes + j*diff_num_nodes;
				auto num_clauses = unsigned int(std::ceil(num_nodes * ratio));

				std::cout << "Num nodes: " << num_nodes
					<< "  Num clauses: " << num_clauses << std::endl;

				vec_y.push_back(
					frac_satisfiable(
						solver, num_nodes, num_clauses, num_average));

				timer.stop();
				timer.output("Set of num_node/num_clause");

			}

			vec_ys.push_back(std::move(vec_y));

		}

		return XYData<double, double>(
			std::make_pair(x_domain, vec_ys));

	}
	
}
