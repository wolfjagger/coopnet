#pragma once

#include <functional>
#include "coopnet/sat/solving/formula.h"
#include "visitor/dpll_visitor.h"



namespace coopnet {

	class Problem;

	class DPLLFormula : public Formula<DPLLVProp, DPLLEProp> {

	private:

		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> greyBuffer;

		bool isContradicting;
		alphali::publisher setContradictPub;
		alphali::subscriber setContradictSub;
		alphali::publisher setUncontradictPub;

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

	};

}
