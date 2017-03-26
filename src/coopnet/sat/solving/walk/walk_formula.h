#pragma once

#include <functional>
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/sat/solving/formula.h"
#include "walk_prop.h"



namespace coopnet {

	class Problem;
	struct BfsWalkVisitor;

	class WalkFormula : public Formula<WalkVProp, WalkEProp> {

	private:

		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> greyBuffer;
		std::shared_ptr<unsigned int> numClausesFailed;

		std::unique_ptr<BfsWalkVisitor> walkVisitor;

	public:

		explicit WalkFormula(const Problem& prob);

		WalkFormula(const WalkFormula& other) = default;
		WalkFormula& operator=(const WalkFormula& other) = default;

		WalkFormula(WalkFormula&& other) = default;
		WalkFormula& operator=(WalkFormula&& other) = default;

		~WalkFormula();



		void flip_node(Node node);

		bool is_SAT() const;

	};

}
