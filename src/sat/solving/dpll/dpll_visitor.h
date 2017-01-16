#pragma once

#include "sat/sat_visitor.h"
#include "sat/component/assignment.h"



namespace sat {

	// Vert visitor? Edge visitor? Visitor list?
	struct dpll_visitor :
		public sat_edge_visitor<dpll_visitor> {
	
		using event_filter = boost::on_examine_edge;

		explicit dpll_visitor() {
			
		}

		// Triggered when edge is encountered
		void edge_event(sat::edge_prop& edge_property,
			vertex_descriptor vert_node, vertex_descriptor vert_clause) {

		}

	};



	struct remove_unit_visitor :
		public sat_vert_visitor<remove_unit_visitor> {
	
		using event_filter = boost::on_examine_edge;

		explicit remove_unit_visitor() {
			
		}

		// Triggered when edge is encountered
		void node_event(sat::vert_prop& vert_property) {
			
		}

		void clause_event(sat::vert_prop& vert_property) {
			
		}

	};

}
