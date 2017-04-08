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

	protected:
		const WalkFormula& form;

		WalkNodeChooser(const WalkFormula& initForm);

	public:

		static std::unique_ptr<WalkNodeChooser> create(
			const WalkFormula& form, WalkNodeChoiceMode mode);

		VertDescriptor choose();

	protected:

		virtual VertDescriptor do_choose() = 0;

	};





	class RandWalkNodeChooser : public WalkNodeChooser {

	public:
		RandWalkNodeChooser(const WalkFormula& initForm);

	protected:
		VertDescriptor do_choose() override;

	};


	class GSATNodeChooser : public WalkNodeChooser {

	private:
		// Nodes of the graph, sorted so that the largest breakCount
		//  nodes are at the front
		std::vector<VertDescriptor> sortedNodes;

	public:
		GSATNodeChooser(const WalkFormula& initForm);

	protected:
		VertDescriptor do_choose() override;

	};


	class UnsatClauseMCNodeChooser : public WalkNodeChooser {

	public:
		double greedyProb;

	public:
		UnsatClauseMCNodeChooser(
			const WalkFormula& initForm, double initGreedyProb = 0.5);

	protected:
		VertDescriptor do_choose() override;

	};

}
