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
		const DPLLFormula* form;

		DPLLNodeChooser();
		DPLLNodeChooser(const DPLLFormula& initForm);

	public:

		virtual ~DPLLNodeChooser() = 0;

		static std::unique_ptr<DPLLNodeChooser>
			create(DPLLNodeChoiceMode mode);

		void set_formula(const DPLLFormula& form);

		boost::optional<DPLLNodeChoice> choose();

	protected:

		virtual void do_set_formula() { }

		virtual DPLLNodeChoice do_choose() = 0;

	};



	class NextNodeChooser : public DPLLNodeChooser {

	public:
		NextNodeChooser() = default;
		NextNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};


	class RandNodeChooser : public DPLLNodeChooser {

	public:
		RandNodeChooser() = default;
		RandNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};


	class MaxSameClauseNodeChooser : public DPLLNodeChooser {

	public:
		MaxSameClauseNodeChooser() = default;
		MaxSameClauseNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};


	class MaxTotClauseNodeChooser : public DPLLNodeChooser {

	public:
		MaxTotClauseNodeChooser() = default;
		MaxTotClauseNodeChooser(const DPLLFormula& form);

	protected:
		DPLLNodeChoice do_choose() override;

	};
	
}
