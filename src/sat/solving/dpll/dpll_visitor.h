#pragma once

#include "boost/graph/breadth_first_search.hpp"
#include "sat/sat_visitor.h"
#include "sat/assignment/assignment.h"
#include "prune_stack.h"



namespace sat {

	// I shouldn't do this as a add/remove verts and edges.
	//  Unfortunately, that will mess up vertex/edge_descriptors.
	//  Instead, I should have another property map that contains
	//  the removal state of verts & edges.
	enum class dpll_vert_status {
		Active, Inactive, SetToTrue, SetToFalse
	};
	using dpll_vert_status_map
		= std::map<vertex_descriptor, dpll_vert_status>;
	using dpll_vert_status_prop_map
		= boost::associative_property_map<dpll_vert_status_map>;

	enum class dpll_edge_status {
		Active, Inactive, PushNodeToClause, PushClauseToNode
	};
	using dpll_edge_status_map
		= std::map<edge_descriptor, dpll_edge_status>;
	using dpll_edge_status_prop_map
		= boost::associative_property_map<dpll_edge_status_map>;

	using dpll_color_map
		= std::map<vertex_descriptor, default_color_type>;
	using dpll_color_prop_map
		= boost::associative_property_map<dpll_color_map>;

	struct dpll_prop_maps {

		dpll_vert_status_prop_map vert_status_map;
		dpll_edge_status_prop_map edge_status_map;
		dpll_color_prop_map color_map;

		dpll_prop_maps(
			dpll_vert_status_prop_map init_vert_status_map,
			dpll_edge_status_prop_map init_edge_status_map,
			dpll_color_prop_map init_color_map) :
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
		incomplete_assignment& assignment;
		prune_stack& prune_action_stack;
		dpll_prop_maps maps;

	public:

		explicit dpll_begin_vert_visitor(
			incomplete_assignment& assignment,
			prune_stack& prune_action_stack,
			dpll_prop_maps maps) :
			assignment(assignment),
			prune_action_stack(prune_action_stack),
			maps(maps) {}

		void node_event(
			const graph& g, vertex_descriptor node,
			const vert_prop& prop);

		void clause_event(
			const graph& g, vertex_descriptor clause,
			const vert_prop& prop);

	private:

		void remove_vert(const graph& g, vertex_descriptor vert);

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
		using event_filter = boost::on_examine_vertex;

	private:
		const incomplete_assignment& assignment;
		prune_stack& prune_action_stack;
		dpll_prop_maps maps;

	public:

		explicit dpll_edge_visitor(
			const incomplete_assignment& assignment,
			prune_stack& prune_action_stack,
			dpll_prop_maps init_maps) :
			assignment(assignment),
			prune_action_stack(prune_action_stack),
			maps(maps) {}

		void edge_event(
			const graph& g, edge_descriptor edge,
			edge_prop& prop,
			vertex_descriptor node, vertex_descriptor clause);

	};



	using dpll_visitor_tuple = 
		std::pair<dpll_begin_vert_visitor, dpll_edge_visitor>;



	class dpll_visitor :
		public boost::bfs_visitor<dpll_visitor_tuple> {

	public:

		dpll_visitor(
			incomplete_assignment& assignment,
			prune_stack& prune_action_stack,
			dpll_prop_maps init_maps);

	};
	
}
