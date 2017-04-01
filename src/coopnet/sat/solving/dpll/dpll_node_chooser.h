#pragma once

#include <memory>
#include "boost/optional.hpp"
#include "coopnet/graph/graph.h"
#include "dpll_node_choice.h"



namespace coopnet {

	class DPLLFormula;

	class DPLLNodeChooser {

	protected:

		using VertChoice = std::pair<VertDescriptor, bool>;

	public:

		boost::optional<DPLLNodeChoice> choose(const DPLLFormula& form);

	protected:

		virtual VertChoice do_choose(const DPLLFormula& form) = 0;

	};



	class NextNodeChooser : public DPLLNodeChooser {

	protected:
		VertChoice do_choose(const DPLLFormula& form) override;

	};


	class RandNodeChooser : public DPLLNodeChooser {

	protected:
		VertChoice do_choose(const DPLLFormula& form) override;

	};


	class MaxSameClauseNodeChooser : public DPLLNodeChooser {

	protected:
		VertChoice do_choose(const DPLLFormula& form) override;

	};


	class MaxTotClauseNodeChooser : public DPLLNodeChooser {

	protected:
		VertChoice do_choose(const DPLLFormula& form) override;

	};



	enum class DPLLNodeChoiceMode {
		Next, Random, MostSameClauses, MostTotClauses
	};
	


	inline std::unique_ptr<DPLLNodeChooser>
		create_dpll_node_chooser(DPLLNodeChoiceMode mode) {

		switch (mode) {
		case DPLLNodeChoiceMode::Next:
			return std::make_unique<NextNodeChooser>();
		case DPLLNodeChoiceMode::Random:
			return std::make_unique<RandNodeChooser>();
		case DPLLNodeChoiceMode::MostSameClauses:
			return std::make_unique<MaxSameClauseNodeChooser>();
		case DPLLNodeChoiceMode::MostTotClauses:
			return std::make_unique<MaxTotClauseNodeChooser>();
		default:
			throw std::exception("Unknown DPLL node choice mode.");
		}

	}

}
