#pragma once

#include <queue>
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/visitor/sat_visitor.h"
#include "dpll_status.h"
#include "prune_stack.h"



namespace sat {

	using incomplete_assignment_prop_map
		= boost::associative_property_map<incomplete_assignment::map>;

	using dpll_vert_status_map
		= std::map<vertex_descriptor, dpll_vert_status>;
	using dpll_vert_status_prop_map
		= boost::associative_property_map<dpll_vert_status_map>;

	using dpll_edge_status_map
		= std::map<edge_descriptor, dpll_edge_status>;
	using dpll_edge_status_prop_map
		= boost::associative_property_map<dpll_edge_status_map>;

	using dpll_color_map
		= std::map<vertex_descriptor, default_color_type>;
	using dpll_color_prop_map
		= boost::associative_property_map<dpll_color_map>;

	struct dpll_prop_maps {

		incomplete_assignment_prop_map partial_assignment_map;
		dpll_vert_status_prop_map vert_status_map;
		dpll_edge_status_prop_map edge_status_map;
		dpll_color_prop_map color_map;

		dpll_prop_maps() = default;

		dpll_prop_maps(
			incomplete_assignment_prop_map init_partial_assignment_map,
			dpll_vert_status_prop_map init_vert_status_map,
			dpll_edge_status_prop_map init_edge_status_map,
			dpll_color_prop_map init_color_map) :
			partial_assignment_map(init_partial_assignment_map),
			vert_status_map(init_vert_status_map),
			edge_status_map(init_edge_status_map),
			color_map(init_color_map) {}

	};



	// This visitor will decide if the vert should be removed:
	//  either if it only has one edge (clause)
	//  or if it has all edges with same sign (node).
	// It also needs to color the surrounding edges if they
	//  should be (re)visited (i.e. if vert is to be removed).
	class dpll_begin_vert_visitor :
		public sat_vert_visitor<dpll_begin_vert_visitor> {
	
	public:
		using event_filter = boost::on_examine_vertex;

	private:
		prune_stack& prune_action_stack;
		bool& is_contradicting;
		dpll_prop_maps maps;

	public:

		explicit dpll_begin_vert_visitor(
			prune_stack& prune_action_stack,
			bool& is_contradicting,
			dpll_prop_maps maps) :
			prune_action_stack(prune_action_stack),
			is_contradicting(is_contradicting),
			maps(maps) {}

		void node_event(
			const graph& g, vertex_descriptor node,
			const vert_prop& prop);

		void clause_event(
			const graph& g, vertex_descriptor clause,
			const vert_prop& prop);

	private:

		void select_node(const graph& g, vertex_descriptor node, bool sgn);
		void satisfy_clause(const graph& g, vertex_descriptor clause);

		void change_assignment(
			vertex_descriptor node, boost::logic::tribool value);
		void change_vert_status(
			vertex_descriptor vert, dpll_vert_status new_status);
		void change_edge_status(
			edge_descriptor edge, dpll_edge_status new_status);

	};



	// This has three responsibilities.
	// (1) color unvisited vertex to visit it
	// (2) it needs to propagate removals to the next vertices,
	//  with cases depending on the circumstances:
	//  (a) node = a => remove clause iff sgn(edge) == a
	//  (b) clause => set node = sgn(edge) iff clause.num_edges == 1
	// (3) remove edge
	class dpll_edge_visitor :
		public sat_edge_visitor<dpll_edge_visitor> {
	
	public:
		using event_filter = boost::on_examine_edge;

	private:
		prune_stack& prune_action_stack;
		bool& is_contradicting;
		dpll_prop_maps maps;

	public:

		explicit dpll_edge_visitor(
			prune_stack& prune_action_stack,
			bool& is_contradicting,
			dpll_prop_maps maps) :
			prune_action_stack(prune_action_stack),
			is_contradicting(is_contradicting),
			maps(maps) {}

		void edge_event(
			const graph& g, edge_descriptor edge,
			const edge_prop& prop,
			vertex_descriptor node, vertex_descriptor clause);

	private:

		void change_vert_status(
			vertex_descriptor vert, dpll_vert_status new_status);
		void change_edge_status(
			edge_descriptor edge, dpll_edge_status new_status);

	};


	// This visitor colors remaining verts grey and in the
	//  queue to black and pops the queue.
	class dpll_finish_vert_visitor :
		public boost::base_visitor<dpll_finish_vert_visitor> {
	
	public:
		using event_filter = boost::on_finish_vertex;

	private:
		boost::queue<vertex_descriptor>& grey_buffer;
		bool& is_contradicting;
		dpll_prop_maps maps;

	public:

		explicit dpll_finish_vert_visitor(
			boost::queue<vertex_descriptor>& grey_buffer,
			bool& is_contradicting,
			dpll_prop_maps maps) :
			grey_buffer(grey_buffer),
			is_contradicting(is_contradicting),
			maps(maps) {}

		template<class Vertex, class Graph>
		void operator()(Vertex v, Graph& g) {

			if (is_contradicting) {
				while (!grey_buffer.empty()) {
					auto vert = grey_buffer.front();
					maps.color_map[vert] = default_color_type::black_color;
					grey_buffer.pop();
				}
			}

		}

	};



	using dpll_visitor_tuple = 
		std::pair<dpll_begin_vert_visitor,
		std::pair<dpll_edge_visitor, dpll_finish_vert_visitor>>;



	class dpll_visitor :
		public boost::bfs_visitor<dpll_visitor_tuple> {

	public:
		bool is_contradicting;

	public:

		dpll_visitor(
			prune_stack& prune_action_stack,
			boost::queue<vertex_descriptor>& grey_queue,
			dpll_prop_maps init_maps);

	};
	
}
