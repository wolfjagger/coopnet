#pragma once

#include <string>
#include "coopnet/sat/solving/walk/walk_node_chooser.h"



namespace coopplot {

	void graph_satisfiability(
		const std::string& foldername, bool withTime = true);

	void graph_multi_satisfiability(
		const std::string& foldername, bool withTime = true);

	void graph_walk_wrt_greedy_prob(
		const std::string& foldername, bool withTime = true);

}
