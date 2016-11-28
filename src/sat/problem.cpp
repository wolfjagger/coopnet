#include "problem.h"
#include <list>

using namespace sat;



template<typename iterator>
problem::problem(int num_nodes, iterator clause_init_beg, iterator clause_init_end) {

	auto list_nodes = std::list<int>(num_nodes);
	nodes = nodelist(list_nodes.begin(), list_nodes.end());
	for (auto i = 0; i < num_nodes; ++i) nodes.emplace_back(i);
	
	for (auto it = clause_init_beg; it < clause_init_end; ++it) {
		add_clause(*it);
	}

}



void problem::add_clause(clause::init clause_init) {
	clauses.emplace(std::move(clause_init));
}

Graph problem::print_graph() const {
	
	// Build graph

	return Graph();

}
