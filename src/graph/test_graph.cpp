#include "test_graph.h"
#include "sat/problem.h"



namespace JJGraph {

	Graph TestGraph() {

		using sat::clause_list;
		using sat::clause_data;
		using sat::problem;

		auto clause_init = std::vector<clause_data>();

		auto nodes = clause_data::node_storage{ 0, 1, 2 };
		auto sgns = clause_data::sgn_storage{ true, true, true };
		clause_init.emplace_back(nodes, sgns);
		nodes = clause_data::node_storage{ 2, 3, 4 };
		sgns = clause_data::sgn_storage{ false, true, true };
		clause_init.emplace_back(nodes, sgns);

		auto prob = problem(5, clause_init.begin(), clause_init.end());
		auto graph = prob.build_graph();

		return graph;

	}

	dynamic_properties TestProps(Graph& graph) {
		
		dynamic_properties dp;
		dp.property("name", get(vertex_color_t::vertex_color, graph));
		dp.property("sign", get(edge_weight_t::edge_weight, graph));

		return dp;

	}

}
