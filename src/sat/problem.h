#pragma once

#include "component/node.h"
#include "component/clause.h"
#include "graph.h"



namespace sat {

	class problem {

	private:

		// We need a good storage solution that can put nodes and clauses
		//  small distance from eachother when they are accessed.
		//  e.g. we want the nodes in a clause stored relatively close to
		//  the clause. Perhaps along a semi-metric?

		graph prob_graph;
		dynamic_properties dyn_props;

	public:

		template<typename iterator>
		problem(int num_nodes,
			iterator clause_init_beg, iterator clause_init_end) {

			auto nodes = node_list(num_nodes);
			auto clauses = clause_list(clause_init_beg, clause_init_end);

			build_graph(std::move(nodes), std::move(clauses));
			
		}



		const graph& get_graph() const;
		const dynamic_properties& get_props() const;



	private:

		void build_graph(node_list&& nodes, clause_list&& clauses);

	};

}
