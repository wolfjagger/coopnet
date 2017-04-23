#pragma once

#include <string>
#include "coopnet/sat/solving/walk/walk_node_chooser.h"



namespace coopplot {

	void graph_satisfiability(
		const std::string& foldername, bool withTime = true);

	void graph_multi_satisfiability(
		const std::string& foldername, bool withTime = true);

	void graph_walk_wrt_greedy_prob(
		int numNodes = 150, int numClauses = 600,
		int numSolves = 100, int numProbs = 11,
		coopnet::WalkNodeChoiceMode nodeChoiceMode
		    = coopnet::WalkNodeChoiceMode::UnsatClauseMC);

}
