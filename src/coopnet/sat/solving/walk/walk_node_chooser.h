#pragma once

#include <memory>
#include "coopnet/graph/base/graph.h"
#include "coopnet/sat/component/component_fwd.h"



namespace coopnet {

	class WalkFormula;

	enum class WalkNodeChoiceMode {
		Random
	};



	class WalkNodeChooser {

	public:

		VertDescriptor choose(const WalkFormula& form);

		static std::unique_ptr<WalkNodeChooser> create(WalkNodeChoiceMode mode);

	protected:

		virtual VertDescriptor do_choose(const WalkFormula& form) = 0;

	};





	class RandWalkNodeChooser : public WalkNodeChooser {

	protected:
		VertDescriptor do_choose(const WalkFormula& form) override;

	};

	class GSATNodeChooser : public WalkNodeChooser {

	protected:
		VertDescriptor do_choose(const WalkFormula& form) override;

	};

	class UnsatClauseMCNodeChooser : public WalkNodeChooser {

	private:
		double greedyProb;

	public:
		UnsatClauseMCNodeChooser(double initGreedyProb = 0.5);

	protected:
		VertDescriptor do_choose(const WalkFormula& form) override;

	private:
		void set_greedy_prob(double newGreedyProb);

	};

}
