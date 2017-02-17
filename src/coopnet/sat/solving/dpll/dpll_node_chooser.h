#pragma once

#include <memory>
#include "coopnet/sat/solving/node_chooser.h"



namespace sat {

	enum class dpll_node_choice_mode {
		Next, Last, Random
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
		default:
			return std::unique_ptr<node_chooser>();
		}

	}

	// Note: other dpll_node_choosers deriving from node_chooser will go here,
	//  and we retain the .cpp to put their definitions.

}
