#pragma once

#include <memory>
#include "coopnet/sat/solving/node_chooser.h"



namespace sat {

	enum class DPLLNodeChoiceMode {
		Next, Last, Random, MostClausesSat
	};
	


	class DPLLNodeChooser : public NodeChooser {

	protected:

		VertChoice do_choose(
			const Formula& form, const AssignmentMap& assign_map) override;

	};



	inline std::unique_ptr<NodeChooser>
		create_dpll_node_chooser(DPLLNodeChoiceMode mode) {

		switch (mode) {
		case DPLLNodeChoiceMode::Next:
			return std::make_unique<NextNodeChooser>();
		case DPLLNodeChoiceMode::Last:
			return std::make_unique<LastNodeChooser>();
		case DPLLNodeChoiceMode::Random:
			return std::make_unique<RandNodeChooser>();
		case DPLLNodeChoiceMode::MostClausesSat:
			return std::make_unique<DPLLNodeChooser>();
		default:
			return std::unique_ptr<NodeChooser>();
		}

	}

}
