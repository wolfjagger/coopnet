#include "clause.h"
#include "algorithm"

using namespace sat;



clause_data::clause_data(node_storage node_init, sgn_storage sgn_init) {
	
	nodes = node_init;
	sgns = sgn_init;

}




clause::clause(clause_data init) :
	data(init) {

}



// Comparison operator for clauses; needed for storage in ordered containers, e.g. set, map
bool sat::operator<(const clause& clause_1, const clause& clause_2) {
	
	// If node_id containers are different, return based on that.
	if (clause_1.data.nodes < clause_2.data.nodes) return true;
	if (clause_2.data.nodes < clause_1.data.nodes) return false;

	// If node_id containers are the same, return based on sgn container.
	return clause_1.data.sgns < clause_2.data.sgns;

}

bool sat::operator==(const clause& clause_1, const clause& clause_2) {
	
	// If node_id containers are different, return based on that.
	if (clause_1.data.nodes != clause_2.data.nodes) return false;

	// If node_id containers are the same, return based on sgn container.
	return clause_1.data.sgns == clause_2.data.sgns;

}





clause_list::clause_list() {
	
}


void clause_list::emplace(clause_data&& init) {
	push(clause(std::move(init)));
}

void clause_list::sort() {
	std::sort(begin(), end());
}

size_t clause_list::size() const {
	return clauses.size();
}


void clause_list::push(clause clause) {
	
	clauses.push_back(clause);

}
