#pragma once

#include <memory>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/component/component_fwd.h"



namespace coopnet {

	class WalkFormula;

	enum class WalkNodeChoiceMode {
		Rand
	};



	class WalkNodeChooser {

	public:

		Node choose(const WalkFormula& form);

		static std::unique_ptr<WalkNodeChooser> create(WalkNodeChoiceMode mode);

	protected:

		virtual VertDescriptor do_choose(const WalkFormula& form) = 0;

	};





	class RandWalkNodeChooser : public WalkNodeChooser {

	protected:

		VertDescriptor do_choose(const WalkFormula& form) override;

	};

}
