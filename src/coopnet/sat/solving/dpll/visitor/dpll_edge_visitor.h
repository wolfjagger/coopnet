#pragma once

#include "alphali/designs/pubsub/collaborator.h"
#include "coopnet/sat/solving/dpll/dpll_prop_maps.h"
#include "coopnet/sat/visitor/pruning_sat_visitor.h"



namespace coopnet {

	// This has three responsibilities.
	// (1) color unvisited vertex to visit it
	// (2) it needs to propagate removals to the next vertices,
	//  with cases depending on the circumstances:
	//  (a) node = a => remove clause iff sgn(edge) == a
	//  (b) clause => set node = sgn(edge) iff clause.num_edges == 1
	// (3) remove edge
	class DPLLEdgeVisitor :
		public PruningSatEdgeVisitor<DPLLEdgeVisitor> {

	public:
		using event_filter = boost::on_examine_edge;

	private:

		DPLLPropMaps maps;

		bool isContradicting;
		alphali::collaborator contradictionCollab;

	public:

		DPLLEdgeVisitor(
			PruneStack& initPruneStack,
			alphali::collaborator&& initContradictionCollab,
			alphali::collaborator& mainContradictCollab,
			alphali::publisher& mainUncontradictPub,
			DPLLPropMaps initMaps);

		void edge_event(
			const MutableSatGraph& g, EdgeDescriptor edge,
			const MutableSatEProp& prop,
			VertDescriptor node, VertDescriptor clause) {

			if (isContradicting) {
				default_edge_event(g, edge, prop, node, clause);
			} else {
				dpll_edge_event(g, edge, prop, node, clause);
			}

		}

	private:

		void dpll_edge_event(
			const MutableSatGraph& g, EdgeDescriptor edge,
			const MutableSatEProp& prop,
			VertDescriptor node, VertDescriptor clause);

		void default_edge_event(
			const MutableSatGraph& g, EdgeDescriptor edge,
			const MutableSatEProp& prop,
			VertDescriptor node, VertDescriptor clause);



		void deactivate_edge(EdgeDescriptor edge, const MutableSatEProp& prop);

		void change_vert_status(
			VertDescriptor vert, DPLLVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, DPLLEdgeStatus new_status);



		void set_contradicting();
		void set_uncontradicting();

	};

}
