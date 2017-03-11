#pragma once

#include <memory>
#include "coopnet/sat/solving/node_chooser.h"
#include "visitor/dpll_choose_node_visitor.h"
#include "dpll_formula.h"



namespace coopnet {

	enum class DPLLNodeChoiceMode {
		Next, Random, MostClausesSat
	};
	


	template<class ConcreteDPLLFormula>
	class DPLLNodeChooser : public NodeChooser<ConcreteDPLLFormula> {

	protected:

		VertChoice do_choose(const ConcreteDPLLFormula& form) override {

			auto visitor = DPLLChooseNodeVisitor();

			form.visit_active_graph(visitor);

			return visitor.retreive_choice();

		}

	};



	inline std::unique_ptr<NodeChooser<DPLLFormula>>
		create_dpll_node_chooser(DPLLNodeChoiceMode mode) {

		switch (mode) {
		case DPLLNodeChoiceMode::Next:
			return std::make_unique<NextNodeChooser<DPLLFormula>>();
		case DPLLNodeChoiceMode::Random:
			return std::make_unique<RandNodeChooser<DPLLFormula>>();
		case DPLLNodeChoiceMode::MostClausesSat:
			return std::make_unique<DPLLNodeChooser<DPLLFormula>>();
		default:
			throw std::exception("Unknown DPLL node choice mode.");
		}

	}

}
