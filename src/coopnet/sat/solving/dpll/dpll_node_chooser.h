#pragma once

#include <memory>
#include "boost/optional.hpp"
#include "coopnet/graph/base/graph.h"
#include "dpll_node_choice.h"



namespace coopnet {

	class DPLLFormula;



	enum class DPLLNodeChoiceMode {
		Next, Random, MostSameClauses, MostTotClauses
	};

	class DPLLNodeChooser {

	protected:
		const DPLLFormula& form;

		DPLLNodeChooser(const DPLLFormula& initForm);

	public:

		static std::unique_ptr<DPLLNodeChooser>
			create(const DPLLFormula& form, DPLLNodeChoiceMode mode);

		boost::optional<DPLLNodeChoice> choose();

	protected:

		virtual DPLLNodeChoice do_choose() = 0;

	};



	class NextNodeChooser : public DPLLNodeChooser {

	public:
		NextNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};


	class RandNodeChooser : public DPLLNodeChooser {

	public:
		RandNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};


	class MaxSameClauseNodeChooser : public DPLLNodeChooser {

	public:
		MaxSameClauseNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};


	class MaxTotClauseNodeChooser : public DPLLNodeChooser {

	public:
		MaxTotClauseNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};
	
}
