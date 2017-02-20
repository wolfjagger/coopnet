#pragma once

#include <deque>
#include <vector>
#include "boost/container/flat_map.hpp"
#include "node.h"



namespace sat {

	// Clause main class
	struct Clause {

	public:
		using LitStorage = boost::container::flat_map<Node, bool>;

	private:
		LitStorage lits;

	public:

		// Constructor
		explicit Clause(LitStorage init) :
			lits(init) { }

		template<typename Iterator>
		Clause(Iterator first, Iterator last) {
			for (auto iter = first; iter != last; ++iter) {
				lits.emplace(iter->n, iter->sgn);
			}
		}

		Clause(std::initializer_list<Literal> init) :
			Clause(init.begin(), init.end()) { }



		const LitStorage& literals() const {
			return lits;
		}

		// Number of nodes
		size_t size() const {
			return lits.size();
		}

	};

	// Comparison operator for clauses;
	//  needed for storage in ordered containers, e.g. set, map
	inline bool operator<(const Clause& clause1, const Clause& clause2) {
		return clause1.literals() < clause2.literals();
	}
	// Equality operator
	inline bool operator==(const Clause& clause1, const Clause& clause2) {
		return clause1.literals() == clause2.literals();
	}
	DEFINE_EXTRA_OPS(Clause);

}
