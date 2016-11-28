#pragma once

#include <deque>
#include <vector>



namespace sat {

	namespace clause_types {
		using index = int;
	}



	// Clause main class
	class clause {

	public:

		// Initialization data; alias for now
		using init = const std::vector<int>;

	private:

		// List of ids for nodes
		std::vector<int> node_ids;

	public:

		// Constructor
		explicit clause(init init_node_ids);

		// Destructor
		~clause();



		// Number of nodes
		size_t size() const {
			return node_ids.size();
		}

		// Comparison operator for clauses; needed for storage in ordered containers, e.g. set, map
		friend bool operator<(const clause& clause_1, const clause& clause_2);

	};

	bool operator<(const clause& clause_1, const clause& clause_2);


	// Storage for clauses
	class clauselist {

	public:

		using storage = std::deque<clause>;
		using iterator = storage::iterator;
		using const_iterator = storage::const_iterator;

	private:

		storage clauses;

	public:
		
		// Constructor
		clauselist();

		// Init clause and add to list
		void emplace(clause::init&& clause_init);
		void sort();

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
