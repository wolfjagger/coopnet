#include "dpll_formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



dpll_formula::dpll_formula(const problem& prob) :
	formula(prob),
	contradicting(false),
	prune_action_stack(),
	grey_buffer(),
	vert_status_map(),
	edge_status_map(),
	color_map() {

	auto vert_iter_pair = boost::vertices(get_prob_graph());
	auto edge_iter_pair = boost::edges(get_prob_graph());

	for (auto vert_iter = vert_iter_pair.first;
		vert_iter != vert_iter_pair.second; ++vert_iter) {

		// Initialize all verts to Active
		vert_status_map.emplace(*vert_iter, dpll_vert_status::Active);

		// Initialize all colors to black, since visitors will expand
		//  where they need to and breadth_first_visit will set sources
		//  to gray
		color_map.emplace(*vert_iter, default_color_type::black_color);

	}

	for (auto edge_iter = edge_iter_pair.first;
		edge_iter != edge_iter_pair.second; ++edge_iter) {

		// Initialize all edges to Active
		edge_status_map.emplace(*edge_iter, dpll_edge_status::Active);

	}

	prop_maps = dpll_prop_maps(
		partial_assign.data,
		vert_status_map, edge_status_map, color_map);
	prune_visitor = std::make_unique<dpll_visitor>(
		prune_action_stack, grey_buffer, prop_maps);

}



void dpll_formula::set_node(node_choice choice) {

	// partial_assign set @ node_to_set=value
	// partial_graph remove node_to_set and reduce (unit clauses
	//  & pure literals), supplying the stack to append

	auto vert_node = partial_assign.node_to_vertex_map->left.at(choice.n);

	auto vert_prune_data
		= std::make_pair(vert_node, dpll_vert_status::Active);
	prune_action_stack.data.push(prune_action(vert_prune_data));

	auto status = choice.sgn ? 
		dpll_vert_status::SetToTrue : dpll_vert_status::SetToFalse;
	prop_maps.vert_status_map[vert_node] = status;

	if (DEBUG) std::cout << "Assign node " << choice.n.id <<
		" with vert " << vert_node << " to " << status << std::endl;

	boost::breadth_first_visit(
		get_prob_graph(), vert_node, grey_buffer,
		*prune_visitor, prop_maps.color_map);

}

void dpll_formula::reverse_prune_to_assignment(node n) {

	if(DEBUG) std::cout << "Pruning\n";

	auto vert_node = partial_assign.node_to_vertex_map->left.at(n);

	auto done = false;
	while (!done && !prune_action_stack.data.empty()) {

		auto action = prune_action_stack.data.top();
		prune_action_stack.data.pop();

		using prune_object = prune_action::prune_object;
		switch (action.type) {
		case prune_object::Assignment: {
			auto& incomplete_assignment_data =
				boost::get<incomplete_assignment_prune_data>(
					action.supp_data);
			auto vert = incomplete_assignment_data.first;
			if(DEBUG) std::cout << "Prune assign " << vert << "\n";
			prop_maps.partial_assignment_map[vert]
				= incomplete_assignment_data.second;
			break;
		}
		case prune_object::Vertex: {

			auto& vertex_data =
				boost::get<vertex_prune_data>(action.supp_data);
			auto vert = vertex_data.first;
			auto status = vertex_data.second;

			if(DEBUG)
				std::cout << "Prune vert status " << vert << " " << status << "\n";

			prop_maps.vert_status_map[vert] = status;

			// If vert is prune-to vert, set to done (status is set first, not assignment)
			if (vert == vert_node && status == dpll_vert_status::Active) done = true;

			break;

		}
		case prune_object::Edge: {

			auto& edge_data =
				boost::get<edge_prune_data>(action.supp_data);
			auto edge = edge_data.first;
			auto status = edge_data.second;

			if(DEBUG)
				std::cout << "Prune edge status " << edge << " " << status << "\n";

			prop_maps.edge_status_map[edge] = status;
			break;

		}}

	}

}
