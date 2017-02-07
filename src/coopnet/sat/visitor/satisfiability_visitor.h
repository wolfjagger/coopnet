#pragma once

#include "coopnet/sat/problem/assignment.h"
#include "sat_visitor.h"



namespace sat {

	struct collect_satisfiability_visitor :
		public sat_edge_visitor<collect_satisfiability_visitor> {
	
		using event_filter = boost::on_examine_edge;

		// Shared so that the assignment can be shared btwn visitors
		std::shared_ptr<const assignment> assigned;
		// Shared so that when this visitor is converted to e.g.
        //  a breadth-first visitor, the state can be retrieved.
		std::shared_ptr<clause_satisfiability> satisfiability;

		explicit collect_satisfiability_visitor(
			std::shared_ptr<const assignment> init_assigned) :
			assigned(init_assigned),
			satisfiability(std::make_shared<clause_satisfiability>()) {
			
		}



		void edge_event(const graph& g, edge_descriptor e,
			const sat::edge_prop& edge_property,
			vertex_descriptor vert_node, vertex_descriptor vert_clause) {

			// If sign of literal in clause matches assignment, clause is satisfied
			auto sgn_of_literal = edge_property.sgn;
			auto assigned_val = assigned->data.at(vert_node);
			if(sgn_of_literal == assigned_val) {
				satisfiability->clauses_satisfied.insert(vert_clause);
			}
		
		}



		size_t count_satisfied() const {
			return satisfiability->clauses_satisfied.size();
		}

	};

}
