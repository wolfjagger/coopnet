#pragma once

#include "coopnet/graph/base/graph.h"



namespace coopnet {

	struct DPLLNodeChoice {

		VertDescriptor vertNode;
		bool sgn;

		DPLLNodeChoice(std::pair<VertDescriptor, bool> pair) :
			vertNode(pair.first),
			sgn(pair.second) { }

	};

	struct DPLLNodeChoiceBranch {
		DPLLNodeChoice choice;
		bool is_first_choice;
	};

}
