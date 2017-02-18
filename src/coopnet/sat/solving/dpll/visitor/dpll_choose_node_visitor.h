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

		std::shared_ptr<vertex_descriptor> chosen_node;
		std::shared_ptr<bool> sgn_sat;

		int max_num_clauses_sat;

	public:

		dpll_choose_node_visitor(dpll_prop_maps init_maps);

		void node_event(
			const graph& g, vertex_descriptor node,
			const vert_prop& prop);

		void clause_event(
			const graph& g, vertex_descriptor clause,
			const vert_prop& prop);

		void reset();



		std::pair<vertex_descriptor, bool> retreive_choice() const;

	};

}
