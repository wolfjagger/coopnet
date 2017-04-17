#pragma once

#include "gnu/xy_data.h"
#include "coopnet/sat/solving/dpll/dpll_node_chooser.h"
#include "coopnet/sat/solving/walk/walk_node_chooser.h"



namespace coopplot {

	struct SatReturn {
		XYData<double, double> sat;
		XYData<double, double> time;
	};

	SatReturn create_sat_data(
		int numNodes,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		coopnet::DPLLNodeChoiceMode nodeChoiceMode);

	SatReturn create_sat_data_comparison(
		int numNodes,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		coopnet::DPLLNodeChoiceMode dpllNodeChoiceMode,
		coopnet::WalkNodeChoiceMode walkNodeChoiceMode);

	SatReturn create_multiple_sat_data(
		int startNumNodes, int endNumNodes, int numPlots,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		coopnet::DPLLNodeChoiceMode nodeChoiceMode);

	SatReturn create_multiple_sat_data_comparison(
		int startNumNodes, int endNumNodes, int numPlots,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		coopnet::DPLLNodeChoiceMode dpllNodeChoiceMode,
		coopnet::WalkNodeChoiceMode walkNodeChoiceMode);

}
