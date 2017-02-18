#pragma once

#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/visitor/sat_visitor.h"
#include "coopnet/sat/solving/dpll/dpll_prop_maps.h"



namespace sat {

	class dpll_choose_node_visitor :
		public sat_vert_visitor<dpll_choose_node_visitor> {

	public:
		using event_filter = boost::on_examine_vertex;
		using incomplete_assignment_prop_map
			= boost::associative_property_map<incomplete_assignment::map>;

	private:

		const dpll_prop_maps maps;

		unsigned int max_num_clauses_sat;
		bool sgn_sat;
		vertex_descriptor chosen_node;

	public:

		dpll_choose_node_visitor(dpll_prop_maps init_maps);

		void node_event(
			const graph& g, vertex_descriptor node,
			const vert_prop& prop);

		void clause_event(
			const graph& g, vertex_descriptor clause,
			const vert_prop& prop);

		void reset();



		vertex_descriptor get_chosen_node() const {
			return chosen_node;
		}
	};

}
