#pragma once

#include "coopnet/sat/component/node.h"



namespace coopnet {

	struct DPLLNodeChoice {
		Node n;
		bool sgn;
	};

	struct DPLLNodeChoiceBranch {
		DPLLNodeChoice choice;
		bool is_first_choice;
	};

}
