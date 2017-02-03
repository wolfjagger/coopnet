#pragma once

#include "sat/sat_visitor.h"
#include "sat/graph.h"



namespace sat::test {

	struct mock_sat_vert_visitor
		: public sat_vert_visitor<mock_sat_vert_visitor> {

		void node_event(
			const graph& g, vertex_descriptor node,
			const vert_prop& prop) {

		}

		void clause_event(
			const graph& g, vertex_descriptor clause,
			const vert_prop& prop) {

		}

	};



	class mock_sat_edge_visitor
		: public sat_edge_visitor<mock_sat_edge_visitor> {

		void edge_event(
			const graph& g, edge_descriptor edge,
			const edge_prop& prop,
			vertex_descriptor node, vertex_descriptor clause) {

		}

	};

}
