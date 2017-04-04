#pragma once

#include <memory>
#include "coopnet/sat/solving/dpll/dpll_prop.h"
#include "coopnet/sat/visitor/prune/pruning_sat_visitor.h"



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

		std::shared_ptr<bool> isContradicting;

	public:

		DPLLEdgeVisitor(
			ReverseStack& initReverseStack,
			std::shared_ptr<bool> isContradictingPtr);

		void edge_event(
			const DPLLSatGraph& g, EdgeDescriptor edge,
			const DPLLEProp& prop,
			VertDescriptor node, VertDescriptor clause) {

			if (*isContradicting) {
				default_edge_event(g, edge, prop, node, clause);
			} else {
				dpll_edge_event(g, edge, prop, node, clause);
			}

		}

	private:

		void dpll_edge_event(
			const DPLLSatGraph& g, EdgeDescriptor edge,
			const DPLLEProp& prop,
			VertDescriptor node, VertDescriptor clause);

		void default_edge_event(
			const DPLLSatGraph& g, EdgeDescriptor edge,
			const DPLLEProp& prop,
			VertDescriptor node, VertDescriptor clause);



		void deactivate_edge(EdgeDescriptor edge, const DPLLEProp& prop);

		void change_vert_status(
			VertDescriptor vert, const DPLLVProp& prop, DPLLVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, const DPLLEProp& prop, DPLLEdgeStatus new_status);

	};

}
