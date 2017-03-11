#pragma once

#include "alphali/designs/pubsub/collaborator.h"
#include "coopnet/sat/solving/dpll/dpll_prop_maps.h"
#include "coopnet/sat/visitor/pruning_sat_visitor.h"



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

		bool isContradicting;
		alphali::collaborator contradictionCollab;

	public:

		DPLLVertVisitor(
			PruneStack& initPruneStack,
			alphali::collaborator&& initContradictionCollab,
			alphali::publisher& mainContradictPub,
			alphali::publisher& mainUncontradictPub);

		void node_event(
			const DPLLSatGraph& g, VertDescriptor node,
			const DPLLVProp& prop) {

			if (isContradicting) {
				default_vert_event(g, node, prop);
			} else {
				dpll_node_event(g, node, prop);
			}

		}

		void clause_event(
			const DPLLSatGraph& g, VertDescriptor clause,
			const DPLLVProp& prop) {

			if (isContradicting) {
				default_vert_event(g, clause, prop);
			} else {
				dpll_clause_event(g, clause, prop);
			}

		}

	private:

		void dpll_node_event(
			const DPLLSatGraph& g, VertDescriptor node,
			const DPLLVProp& prop);

		void dpll_clause_event(
			const DPLLSatGraph& g, VertDescriptor clause,
			const DPLLVProp& prop);

		void default_vert_event(
			const DPLLSatGraph& g, VertDescriptor vert,
			const DPLLVProp& prop);



		void select_node(const DPLLSatGraph& g,
			VertDescriptor node, const DPLLVProp& prop, bool sgn);
		void satisfy_clause(const DPLLSatGraph& g,
			VertDescriptor clause, const DPLLVProp& prop);

		void deactivate_vert(VertDescriptor vert, const DPLLVProp& prop);
		void deactivate_edge(EdgeDescriptor edge, const DPLLEProp& prop);

		void change_vert_status(
			VertDescriptor vert, const DPLLVProp& prop, DPLLVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, const DPLLEProp& prop, DPLLEdgeStatus new_status);



		void set_contradicting();
		void set_uncontradicting();

	};

}
