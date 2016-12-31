#include "test_graph.h"
#include "sat/problem.h"
#include "sat/problem_factory.h"



namespace jj_graph {

	graph test_graph() {

		auto prob = sat::generate_random_3sat_problem(10, 100);

		auto g = prob.build_graph();

		return g;

	}

	dynamic_properties test_props(graph& g) {
		
		dynamic_properties dp;
		dp.property("name", get(vertex_color_t::vertex_color, g));
		dp.property("sign", get(edge_weight_t::edge_weight, g));

		return dp;

	}

}
