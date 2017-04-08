#pragma once

#include <memory>
#include "coopnet/sat/solving/dpll/dpll_prop.h"
#include "coopnet/sat/visitor/prune/pruning_sat_visitor.h"



namespace coopnet {

	// This visitor will decide if the vert should be removed:
	//  either if it only has one edge (clause)
	//  or if it has all edges with same sign (node).
	// It also needs to color the surrounding edges if they
	//  should be (re)visited (i.e. if vert is to be removed).
	class DPLLVertVisitor :
		public PruningSatVertVisitor<DPLLVertVisitor> {

	public:
		using event_filter = boost::on_examine_vertex;

	private:

		std::shared_ptr<bool> isContradicting;

	public:

		DPLLVertVisitor(
			ReverseStack& initReverseStack,
			std::shared_ptr<bool> isContradictingPtr);

		void node_event(
			const DPLLSatGraph& g, VertDescriptor node,
			const DPLLProp::Node& prop) {

			if (*isContradicting) {
				default_node_event(g, node, prop);
			} else {
				dpll_node_event(g, node, prop);
			}

		}

		void clause_event(
			const DPLLSatGraph& g, VertDescriptor clause,
			const DPLLProp::Clause& prop) {

			if (*isContradicting) {
				default_clause_event(g, clause, prop);
			} else {
				dpll_clause_event(g, clause, prop);
			}

		}

	private:

		void dpll_node_event(
			const DPLLSatGraph& g, VertDescriptor node,
			const DPLLProp::Node& prop);

		void dpll_clause_event(
			const DPLLSatGraph& g, VertDescriptor clause,
			const DPLLProp::Clause& prop);

		void default_node_event(
			const DPLLSatGraph& g, VertDescriptor vert,
			const DPLLProp::Node& prop);

		void default_clause_event(
			const DPLLSatGraph& g, VertDescriptor vert,
			const DPLLProp::Clause& prop);



		void select_node(const DPLLSatGraph& g,
			VertDescriptor node, const DPLLProp::Node& prop, bool sgn);
		void satisfy_clause(const DPLLSatGraph& g,
			VertDescriptor clause, const DPLLProp::Clause& prop);

		void deactivate_node(VertDescriptor vert, const DPLLProp::Node& prop);
		void deactivate_clause(VertDescriptor vert, const DPLLProp::Clause& prop);
		void deactivate_edge(EdgeDescriptor edge, const DPLLProp::Edge& prop);

		void change_node_status(
			VertDescriptor vert, const DPLLProp::Node& prop, DPLLNodeStatus new_status);
		void change_clause_status(
			VertDescriptor vert, const DPLLProp::Clause& prop, DPLLClauseStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, const DPLLProp::Edge& prop, DPLLEdgeStatus new_status);

	};

}
