#pragma once

#include "sat/sat_visitor.h"
#include "sat/graph.h"



namespace sat {

	struct dpll_vert_visitor :
		public sat_vert_visitor<dpll_vert_visitor> {
	
		// If we know to remove, use on_finish_vertex so that it's
		//  after edges (which should also use on_finish_edge to remove)
		using event_filter = boost::on_examine_vertex;

		explicit dpll_vert_visitor() {
			
		}

		void node_event(vertex_descriptor node) {

		}

		void clause_event(vertex_descriptor clause) {

		}

	};

	struct dpll_edge_visitor :
		public sat_edge_visitor<dpll_edge_visitor> {
	
		using event_filter = boost::on_examine_edge;

		explicit dpll_edge_visitor() {
			
		}

		void edge_event(edge_prop& edge_property,
			vertex_descriptor node, vertex_descriptor clause) {

		}

	};
	
}
