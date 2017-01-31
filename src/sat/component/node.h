#pragma once

#include "alphali/util/operators.h"
#include "component_fwd.h"



namespace sat {

	struct node {

		unsigned int id;

		node(unsigned int init);

	};

	bool operator<(const node& node_1, const node& node_2);
	bool operator==(const node& node_1, const node& node_2);
	DEFINE_EXTRA_OPS(node);



	node_list create_nodes(size_t num_nodes);

}
