#include "clause.h"

using namespace sat;



clause::clause(lit_storage lit_init) {
	
	lits = lit_init;

}



// Comparison operator for clauses; needed for storage in ordered containers, e.g. set, map
bool sat::operator<(const clause& clause1, const clause& clause2) {

	return clause1.literals() < clause2.literals();
	
}

bool sat::operator==(const clause& clause1, const clause& clause2) {

	return clause1.literals() == clause2.literals();
	
}
