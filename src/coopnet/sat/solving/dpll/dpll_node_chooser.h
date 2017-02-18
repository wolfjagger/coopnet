#pragma once

#include <memory>
#include "coopnet/sat/solving/node_chooser.h"



namespace sat {

	enum class dpll_node_choice_mode {
		Next, Last, Random, MostClausesSat
	};
	


	class dpll_node_chooser : public node_chooser {

	protected:

		vert_choice do_choose(
			const formula& form, const assignment_map& assign_map) override;

	};



	inline std::unique_ptr<node_chooser>
		create_dpll_node_chooser(dpll_node_choice_mode mode) {

		switch (mode) {
		case dpll_node_choice_mode::Next:
			return std::make_unique<next_node_chooser>();
		case dpll_node_choice_mode::Last:
			return std::make_unique<last_node_chooser>();
		case dpll_node_choice_mode::Random:
			return std::make_unique<rand_node_chooser>();
		case dpll_node_choice_mode::MostClausesSat:
			return std::make_unique<dpll_node_chooser>();
		default:
			return std::unique_ptr<node_chooser>();
		}

	}

}
