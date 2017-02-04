#pragma once

#include "alphali/util/operators.h"
#include "component_fwd.h"



namespace sat {

	struct node {

		unsigned int id;

		node(unsigned int init);

	};

	bool operator<(const node& node1, const node& node2);
	bool operator==(const node& node1, const node& node2);
	DEFINE_EXTRA_OPS(node);

	node_list create_nodes(size_t num_nodes);


	struct literal {

		node n;
		bool sgn;

		literal(node n_init, bool sgn_init);

	};

	bool operator<(const literal& lit1, const literal& lit2);
	bool operator==(const literal& lit1, const literal& lit2);
	DEFINE_EXTRA_OPS(literal);

}