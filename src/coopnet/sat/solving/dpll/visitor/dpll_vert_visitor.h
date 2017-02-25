#pragma once

#include "alphali/designs/pubsub/collaborator.h"
#include "coopnet/sat/solving/dpll/dpll_prop_maps.h"
#include "coopnet/sat/visitor/pruned_sat_visitor.h"



namespace coopnet {

	// This visitor will decide if the vert should be removed:
	//  either if it only has one edge (clause)
	//  or if it has all edges with same sign (node).
	// It also needs to color the surrounding edges if they
	//  should be (re)visited (i.e. if vert is to be removed).
	class DPLLVertVisitor :
		public PruneSatVertVisitor<DPLLVertVisitor> {

	public:
		using event_filter = boost::on_examine_vertex;

	private:

		DPLLPropMaps maps;

		bool isContradicting;
		alphali::collaborator contradictionCollab;

	public:

		DPLLVertVisitor(
			alphali::collaborator&& initContradictionCollab,
			alphali::collaborator& mainContradictCollab,
			alphali::publisher& mainUncontradictPub,
			DPLLPropMaps initMaps);

		void node_event(
			const MutableSatGraph& g, VertDescriptor node,
			const MutableSatVProp& prop) {

			if (isContradicting) {
				default_vert_event(g, node, prop);
			} else {
				dpll_node_event(g, node, prop);
			}

		}

		void clause_event(
			const MutableSatGraph& g, VertDescriptor clause,
			const MutableSatVProp& prop) {

			if (isContradicting) {
				default_vert_event(g, clause, prop);
			} else {
				dpll_clause_event(g, clause, prop);
			}

		}

	private:

		void dpll_node_event(
			const MutableSatGraph& g, VertDescriptor node,
			const MutableSatVProp& prop);

		void dpll_clause_event(
			const MutableSatGraph& g, VertDescriptor clause,
			const MutableSatVProp& prop);

		void default_vert_event(
			const MutableSatGraph& g, VertDescriptor vert,
			const MutableSatVProp& prop);



		void select_node(const MutableSatGraph& g, VertDescriptor node, bool sgn);
		void satisfy_clause(const MutableSatGraph& g, VertDescriptor clause);
		void deactivate_vert(VertDescriptor vert);
		void deactivate_edge(EdgeDescriptor edge);

		void change_vert_status(
			VertDescriptor vert, DPLLVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, DPLLEdgeStatus new_status);



		void set_contradicting();
		void set_uncontradicting();

	};

}
