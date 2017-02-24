#pragma once

#include "coopnet/sat/component/node.h"



namespace coopnet {

	struct NodeChoice {
		Node n;
		bool sgn;
	};

	struct NodeDecision {
		NodeChoice choice;
		bool is_first_choice;
	};

}
