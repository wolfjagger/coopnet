#pragma once

#include <functional>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/solving/formula.h"
#include "visitor/dpll_visitor.h"



namespace coopnet {

	class Problem;

	class DPLLFormula : public Formula<MutableSatVProp, MutableSatEProp> {

	private:

		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> greyBuffer;

		// For property maps associated with vert/edge statuses
		DPLLVertStatusMap vertStatusMap;
		DPLLEdgeStatusMap edgeStatusMap;
		// Vert color map primarily controlled dynamically by visitor
		DPLLColorMap colorMap;

		bool isContradicting;
		alphali::publisher setContradictPub;
		alphali::subscriber setContradictSub;
		alphali::publisher setUncontradictPub;

		DPLLPropMaps propMaps;

		std::unique_ptr<DPLLVisitor> pruneVisitor;

	public:

		explicit DPLLFormula(const Problem& prob);

		DPLLFormula(const DPLLFormula& other) = default;
		DPLLFormula& operator=(const DPLLFormula& other) = default;

		DPLLFormula(DPLLFormula&& other) = default;
		DPLLFormula& operator=(DPLLFormula&& other) = default;

		~DPLLFormula() = default;



		void set_node(NodeChoice choice);

		bool is_contradicting() const;
		void set_contradicting();
		void set_uncontradicting();

		DPLLPropMaps get_prop_maps() {
			return propMaps;
		}
		const DPLLPropMaps get_prop_maps() const {
			return propMaps;
		}

	};

}
