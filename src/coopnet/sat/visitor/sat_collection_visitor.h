#pragma once

#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/problem/problem.h"
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
		std::shared_ptr<const SatGraphTranslator> translator;

		explicit SatCollectionVisitor(
			const Problem& prob,
			std::shared_ptr<const Assignment> init_assigned) :
			translator(prob.get_node_vert_translator()),
			assigned(init_assigned),
			satisfiability(std::make_shared<ClauseSatisfiability>()) {
			
		}



		template<typename SatGraph>
		void edge_event(const SatGraph& g, EdgeDescriptor e,
			const coopnet::BaseSatEProp& edge_property,
			VertDescriptor vertNode, VertDescriptor vertClause) {

			// If sign of literal in clause matches assignment, clause is satisfied
			auto n = translator->vert_to_node(vertNode);
			auto sgn_of_literal = edge_property.base.sgn;
			auto assigned_val = assigned->data.at(n);
			if (sgn_of_literal == assigned_val) {
				satisfiability->clausesSatisfied.insert(vertClause);
			}
		
		}



		size_t count_satisfied() const {
			return satisfiability->clausesSatisfied.size();
		}

	};

}
