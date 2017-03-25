#pragma once

#include <functional>
#include "coopnet/sat/solving/formula.h"
#include "visitor/walk_visitor.h"
#include "walk_prop.h"



namespace coopnet {

	class Problem;

	class WalkFormula : public Formula<WalkVProp, WalkEProp> {

	private:

		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> greyBuffer;

		std::unique_ptr<WalkVisitor> walkVisitor;

	public:

		explicit WalkFormula(const Problem& prob);

		WalkFormula(const WalkFormula& other) = default;
		WalkFormula& operator=(const WalkFormula& other) = default;

		WalkFormula(WalkFormula&& other) = default;
		WalkFormula& operator=(WalkFormula&& other) = default;

		~WalkFormula() = default;



		void flip_node(Node node);

		bool is_SAT_cached() const;

	};

}
