#pragma once

#include "coopnet/sat/component/node.h"



namespace sat {

	struct node_choice {
		node n;
		bool sgn;
	};

	struct node_decision {
		node_choice choice;
		bool is_first_choice;
	};

}
