#pragma once

#include "boost/graph/visitors.hpp"
#include "sat/component/assignment.h"
#include "sat/graph.h"



namespace sat {

	struct collect_satisfiability_visitor :
		public boost::base_visitor<collect_satisfiability_visitor> {
	
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

		// Triggered when edge is encountered
		template<class Edge, class Graph>
		void operator()(Edge e, Graph& g) {

			// Find which vert is node and which is clause
			auto vert_node = boost::source(e, g);
			auto vert_clause = boost::target(e, g);
			if (g[vert_node].kind == vert_prop::Clause) {
				std::swap(vert_node, vert_clause);
			}

			// If sign of literal in clause matches assignment, clause is satisfied
			auto sgn_of_literal = g[e].sgn;
			auto assigned_val = assigned->data.at(vert_node);
			if(sgn_of_literal == assigned_val) {
				satisfiability->clauses_satisfied.insert(vert_clause);
			} else {
				satisfiability->clauses_unsatisfied.insert(vert_clause);
			}
		
		}



		size_t count_satisfied() const {
			return satisfiability->clauses_satisfied.size();
		}

		size_t count_unsatisfied() const {
			return satisfiability->clauses_unsatisfied.size();
		}

	};

}
