#pragma once

#include <functional>
#include "boost/pending/queue.hpp"
#include "coopnet/sat/solving/formula.h"
#include "dpll_prop.h"



namespace coopnet {

	class Problem;
	class BfsDPLLVisitor;

	class DPLLFormula : public Formula<DPLLVProp, DPLLEProp> {

	private:

		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> greyBuffer;

		std::shared_ptr<bool> isContradicting;

		std::unique_ptr<BfsDPLLVisitor> pruneVisitor;

	public:

		explicit DPLLFormula(const Problem& prob);

		DPLLFormula(const DPLLFormula& other) = default;
		DPLLFormula& operator=(const DPLLFormula& other) = default;

		DPLLFormula(DPLLFormula&& other) = default;
		DPLLFormula& operator=(DPLLFormula&& other) = default;

		~DPLLFormula();



		void set_node(NodeChoice choice);

		bool is_contradicting() const;
		void set_contradicting();
		void set_uncontradicting();

	};

}
