#include "clause.h"
#include "algorithm"

using namespace sat;



clause::clause(init init_node_ids)
	: node_ids(init_node_ids) {

}

clause::~clause() {
	
}



// Comparison operator for clauses; needed for storage in ordered containers, e.g. set, map
bool sat::operator<(const clause& clause_1, const clause& clause_2) {

	if (clause_1.size() < clause_2.size()) return true;
	if (clause_1.size() > clause_2.size()) return false;

	for (auto i = 0; i < clause_1.size(); ++i) {
		if (clause_1.node_ids[i] < clause_2.node_ids[i]) return true;
		else if (clause_1.node_ids[i] > clause_2.node_ids[i]) return false;
	}
	return false;

}





clauselist::clauselist() {
	
}


void clauselist::emplace(typename clause::init&& clause_init) {
	push(clause(clause_init));
}

void clauselist::sort() {
	std::sort(begin(), end());
}


void clauselist::push(clause clause) {
	
	clauses.push_back(clause);

}
