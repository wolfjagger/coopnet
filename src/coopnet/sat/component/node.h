#pragma once

#include <set>
#include "alphali/util/operators.h"
#include "component_fwd.h"



namespace coopnet {

	struct Node {

		unsigned int id;

		Node(unsigned int init);

	};

	bool operator<(const Node& node1, const Node& node2);
	bool operator==(const Node& node1, const Node& node2);
	DEFINE_EXTRA_OPS(Node);

	NodeList create_nodes(size_t num_nodes);


	struct Literal {

	private:

		struct compare_lits {
			bool operator()(const Literal& lit1, const Literal& lit2) const {
				return lit1.n < lit2.n;
			}
		};

	public:

		using LitSet = std::set<Literal, compare_lits>;

		Node n;
		bool sgn;

		Literal(Node n_init, bool sgn_init);

	};

	bool operator<(const Literal& lit1, const Literal& lit2);
	bool operator==(const Literal& lit1, const Literal& lit2);
	DEFINE_EXTRA_OPS(Literal);

}
