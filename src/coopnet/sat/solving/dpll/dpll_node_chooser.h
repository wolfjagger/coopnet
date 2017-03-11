#pragma once

#include <memory>
#include "coopnet/sat/solving/node_chooser.h"
#include "dpll_formula.h"



namespace coopnet {

	enum class DPLLNodeChoiceMode {
		Next, Random, MostClausesSat
	};
	


	inline std::unique_ptr<NodeChooser<DPLLFormula>>
		create_dpll_node_chooser(DPLLNodeChoiceMode mode) {

		switch (mode) {
		case DPLLNodeChoiceMode::Next:
			return std::make_unique<NextNodeChooser<DPLLFormula>>();
		case DPLLNodeChoiceMode::Random:
			return std::make_unique<RandNodeChooser<DPLLFormula>>();
		case DPLLNodeChoiceMode::MostClausesSat:
			return std::make_unique<MaxClauseNodeChooser<DPLLFormula>>();
		default:
			throw std::exception("Unknown DPLL node choice mode.");
		}

	}

}
