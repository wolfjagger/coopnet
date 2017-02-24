#pragma once

#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "sat_visitor.h"



namespace coopnet {

	struct SatCollectionVisitor :
		public SatEdgeVisitor<SatCollectionVisitor> {
	
		using event_filter = boost::on_examine_edge;

		// Shared so that the assignment can be shared btwn visitors
		std::shared_ptr<const Assignment> assigned;
		// Shared so that when this visitor is converted to e.g.
        //  a breadth-first visitor, the state can be retrieved.
		std::shared_ptr<ClauseSatisfiability> satisfiability;
		std::shared_ptr<const NodeVertMap> node_to_vertex_map;

		explicit SatCollectionVisitor(
			const Problem& prob,
			std::shared_ptr<const Assignment> init_assigned) :
			node_to_vertex_map(prob.get_node_vert_map()),
			assigned(init_assigned),
			satisfiability(std::make_shared<ClauseSatisfiability>()) {
			
		}



		void edge_event(const BaseSatGraph& g, EdgeDescriptor e,
			const coopnet::EdgeProp& edge_property,
			VertDescriptor vert_node, VertDescriptor vert_clause) {

			// If sign of literal in clause matches assignment, clause is satisfied
			auto n = node_to_vertex_map->right.at(vert_node);
			auto sgn_of_literal = edge_property.sgn;
			auto assigned_val = assigned->data.at(n);
			if(sgn_of_literal == assigned_val) {
				satisfiability->clauses_satisfied.insert(vert_clause);
			}
		
		}



		size_t count_satisfied() const {
			return satisfiability->clauses_satisfied.size();
		}

	};

}
