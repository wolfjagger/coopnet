#pragma once

#include <memory>
#include "coopnet/graph/base/graph.h"
#include "coopnet/sat/component/component_fwd.h"



namespace coopnet {

	class WalkFormula;



	enum class WalkNodeChoiceMode {
		Random, GSAT, UnsatClauseMC
	};

	class WalkNodeChooser {

	protected:
		const WalkFormula* form;

		WalkNodeChooser();
		WalkNodeChooser(const WalkFormula& initForm);

	public:

		static std::unique_ptr<WalkNodeChooser>
			create(WalkNodeChoiceMode mode);

		void set_formula(const WalkFormula& form);

		VertDescriptor choose();

	protected:

		virtual void do_set_formula() { }

		virtual VertDescriptor do_choose() = 0;

	};





	class RandWalkNodeChooser : public WalkNodeChooser {

	public:
		RandWalkNodeChooser() = default;
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
		GSATNodeChooser() = default;
		GSATNodeChooser(const WalkFormula& initForm);

	protected:
		void do_set_formula() override;
		VertDescriptor do_choose() override;

	};


	class UnsatClauseMCNodeChooser : public WalkNodeChooser {

	public:
		double greedyProb;

	public:
		UnsatClauseMCNodeChooser(double initGreedyProb = 0.5);
		UnsatClauseMCNodeChooser(
			const WalkFormula& initForm, double initGreedyProb = 0.5);

	protected:
		VertDescriptor do_choose() override;

	};

}
