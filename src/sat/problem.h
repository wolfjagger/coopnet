#pragma once

#include <list>
#include <map>
#include <numeric>
#include "graph/graphdefs.h"
#include "component/node.h"
#include "component/clause.h"



namespace sat {

	class problem {

	public:

		using node_map = std::map<node, vertex_descriptor>;
		using clause_map = std::map<clause, vertex_descriptor>;

	private:

		node_list nodes;
		clause_list clauses;

		// Note these store duplicate nodes and clauses.
		//  They are small, so prob not a problem.
		//  If necessary, we can remove the above and only use below.
		//  But probably it is more important to iterate quickly
		//  than it is to save space.
		node_map map_nodes;
		clause_map map_clauses;

		// We need a good storage solution that can put nodes and clauses
		//  small distance from eachother when they are accessed.
		//  e.g. we want the nodes in a clause stored relatively close to
		//  the clause. Perhaps along a semi-metric?

		// Probably store Graph here as well.

	public:

		template<typename iterator>
		problem(int num_nodes,
			iterator clause_init_beg, iterator clause_init_end) {

			auto list_nodes = std::list<int>(num_nodes);
			std::iota(list_nodes.begin(), list_nodes.end(), 0);
			nodes = node_list(list_nodes.begin(), list_nodes.end());
			
			for (auto it = clause_init_beg; it != clause_init_end; ++it) {
				add_clause(*it);
			}
			
		}

		void add_clause(clause_data init);

		graph build_graph();



		const node_list& list_nodes() const;
		const clause_list& list_clauses() const;

	};

}
