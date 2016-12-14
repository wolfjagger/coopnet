#pragma once

#include <deque>
#include <vector>
#include "node.h"



namespace sat {

	struct clause_data {

		// Should make clause a template on number of nodes, N.
		//  That way, we can use std::array<node_id, N> and save
		//  a little space.
		using node_storage = std::vector<node>;
		node_storage nodes;

		// Similarly, could switch to std::bitset<N>?
		using sgn_storage = std::vector<bool>;
		sgn_storage sgns;

		//TODO: In addition, probably should store them together as a "literal".
		//  But for now this is fine.

		clause_data(node_storage node_init, sgn_storage sgn_init);

	};


	// Clause main class
	struct clause {

	private:

		clause_data data;

	public:

		// Constructor
		explicit clause(clause_data init);



		const clause_data::node_storage& nodes() const {
			return data.nodes;
		}

		const clause_data::sgn_storage& sgns() const {
			return data.sgns;
		}

		// Number of nodes
		size_t size() const {
			return data.nodes.size();
		}

		friend bool operator<(const clause& clause_1, const clause& clause_2);
		friend bool operator==(const clause& clause_1, const clause& clause_2);

	};

	// Comparison operator for clauses; needed for storage in ordered containers, e.g. set, map
	bool operator<(const clause& clause_1, const clause& clause_2);
	// Equality operator
	bool operator==(const clause& clause_1, const clause& clause_2);



	// Storage for clauses
	class clause_list {

	private:
		using storage = std::deque<clause>;
	public:
		using iterator = storage::iterator;
		using const_iterator = storage::const_iterator;

	private:

		storage clauses;

	public:
		
		// Constructor
		clause_list();

		// Init clause and add to list
		void emplace(clause_data&& init);
		void sort();
		size_t size() const;

	private:

		// Push clause to list
		void push(clause clause);

	public:

		// Give access to clause iteration
		iterator begin() { return clauses.begin(); }
		const_iterator begin() const { return clauses.begin(); }
		const_iterator cbegin() const { return clauses.cbegin(); }
		iterator end() { return clauses.end(); }
		const_iterator end() const { return clauses.end(); }
		const_iterator cend() const { return clauses.cend(); }

	};

}
