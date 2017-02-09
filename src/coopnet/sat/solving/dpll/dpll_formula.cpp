#include "dpll_formula.h"
#include <iostream>
#include "coopnet/sat/problem/problem.h"

using namespace sat;



namespace {
	constexpr bool DEBUG_print_prune = false;
}



dpll_formula::dpll_formula(const problem& prob) :
	prob_graph(std::cref(prob.get_graph())),
	contradicting(false),
	prune_action_stack(),
	grey_buffer(),
	partial_assign(prob),
	vert_status_map(),
	edge_status_map(),
	color_map() {

	auto vert_iter_pair = boost::vertices(prob_graph.get());
	auto edge_iter_pair = boost::edges(prob_graph.get());

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
		prob.get_node_vert_map(), partial_assign.data,
		vert_status_map, edge_status_map, color_map);
	prune_visitor = std::make_unique<dpll_visitor>(
		prune_action_stack, grey_buffer, prop_maps);

}



dpll_formula::dpll_formula(const dpll_formula& other) :
	prob_graph(other.prob_graph) {
	*this = other;
}

dpll_formula& dpll_formula::operator=(const dpll_formula& other) {

	prob_graph = other.prob_graph;
	contradicting = other.contradicting;
	prune_action_stack = other.prune_action_stack;
	grey_buffer = other.grey_buffer;

 	partial_assign = other.partial_assign;
	vert_status_map = other.vert_status_map;
	edge_status_map = other.edge_status_map;
	color_map = other.color_map;

	prop_maps = dpll_prop_maps(
		other.prop_maps.node_to_vertex_map, partial_assign.data,
		vert_status_map, edge_status_map, color_map);

	prune_visitor = std::make_unique<dpll_visitor>(
		prune_action_stack, grey_buffer, prop_maps);
	
	return *this;

}



void dpll_formula::set_node(node n, bool value) {

	// partial_assign set @ node_to_set=value
	// partial_graph remove node_to_set and reduce (unit clauses
	//  & pure literals), supplying the stack to append

	auto vert = prop_maps.node_to_vertex_map.left.at(n);

	prop_maps.vert_status_map[vert] = value ?
		dpll_vert_status::SetToTrue : dpll_vert_status::SetToFalse;

	auto assign_prune_data
		= std::make_pair(vert, boost::logic::indeterminate);
	prune_action_stack.data.push(prune_action(assign_prune_data));

	boost::breadth_first_visit(
		prob_graph.get(), vert, grey_buffer,
		*prune_visitor, prop_maps.color_map);

}

void dpll_formula::reverse_prune_to_assignment(node n) {

	if(DEBUG_print_prune) std::cout << "Pruning\n";

	auto vert_node = prop_maps.node_to_vertex_map.left.at(n);

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
			if(DEBUG_print_prune) std::cout << "Assign " << vert << "\n";
			prop_maps.partial_assignment_map[n]
				= incomplete_assignment_data.second;
			if (vert == vert_node) done = true;
			break;
		}
		case prune_object::Vertex: {
			auto& vertex_data =
				boost::get<vertex_prune_data>(action.supp_data);
			auto vert = vertex_data.first;
			if(DEBUG_print_prune) std::cout << "Vert status " << vert << "\n";
			prop_maps.vert_status_map[vert] = vertex_data.second;
			break;
		}
		case prune_object::Edge: {
			auto& edge_data =
				boost::get<edge_prune_data>(action.supp_data);
			auto edge = edge_data.first;
			if(DEBUG_print_prune) std::cout << "Edge status " << edge << "\n";
			prop_maps.edge_status_map[edge] = edge_data.second;
			break;
		}
		}

	}

}


bool dpll_formula::is_SAT() const {

	auto is_SAT_pred =
		[](std::pair<node, boost::logic::tribool> pair) {
		return boost::logic::indeterminate(pair.second);
	};

	return std::none_of(
		partial_assign.data.cbegin(), partial_assign.data.cend(), is_SAT_pred);

}
