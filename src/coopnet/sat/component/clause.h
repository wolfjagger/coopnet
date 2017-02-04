#pragma once

#include <deque>
#include <vector>
#include "boost/container/flat_map.hpp"
#include "node.h"



namespace sat {

	// Clause main class
	struct clause {

	public:
		using lit_storage = boost::container::flat_map<node, bool>;

	private:
		lit_storage lits;

	public:

		// Constructor
		explicit clause(lit_storage init) :
			lits(init) { }

		template<typename iterator>
		clause(iterator first, iterator last) {
			for (auto iter = first; iter != last; ++iter) {
				lits.emplace(iter->n, iter->sgn);
			}
		}

		clause(std::initializer_list<literal> init) :
			clause(init.begin(), init.end()) { }



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
	inline bool operator<(const clause& clause1, const clause& clause2) {
		return clause1.literals() < clause2.literals();
	}
	// Equality operator
	inline bool operator==(const clause& clause1, const clause& clause2) {
		return clause1.literals() == clause2.literals();
	}
	DEFINE_EXTRA_OPS(clause);

}
