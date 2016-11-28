#pragma once

#include "node.h"
#include "clause.h"
#include "graph/graphdefs.h"



namespace sat {

	class problem {

	private:

		nodelist nodes;
		clauselist clauses;

	public:

		template<typename iterator>
		problem(int num_nodes, iterator clause_init_beg, iterator clause_init_end);

		void add_clause(clause::init clause_init);

		Graph print_graph() const;

	};

}