#pragma once

#include <functional>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/solving/formula.h"
#include "visitor/dpll_visitor.h"



namespace sat {

	class Problem;

	class DPLLFormula : public Formula {

	private:

		bool isContradicting;
		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> greyBuffer;

		// For property maps associated with vert/edge statuses
		DPLLVertStatusMap vertStatusMap;
		DPLLEdgeStatusMap edgeStatusMap;
		// Vert color map primarily controlled dynamically by visitor
		DPLLColorMap colorMap;

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

		bool is_contradicting() const {
			return pruneVisitor->isContradicting;
		}
		void set_contradicting(bool value) {
			pruneVisitor->isContradicting = value;
		}

		DPLLPropMaps get_prop_maps() {
			return propMaps;
		}
		const DPLLPropMaps get_prop_maps() const {
			return propMaps;
		}

	};

}
