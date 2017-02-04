#pragma once

#include <deque>
#include <vector>
#include "alphali/util/operators.h"
#include "node.h"



namespace sat {

	// Clause main class
	struct clause {

	public:
		// Should make clause a template on number of nodes, N.
		//  That way, we can use std::array<literal, N> and save
		//  a little space.
		using lit_storage = std::vector<literal>;

	private:
		lit_storage lits;

	public:

		// Constructor
		explicit clause(lit_storage init);



		const lit_storage& literals() const {
			return lits;
		}

		// Number of nodes
		size_t size() const {
			return lits.size();
		}

	};

	// Comparison operator for clauses;
	//  needed for storage in ordered containers, e.g. set, map
	bool operator<(const clause& clause1, const clause& clause2);
	// Equality operator
	bool operator==(const clause& clause1, const clause& clause2);
	DEFINE_EXTRA_OPS(clause);

}
