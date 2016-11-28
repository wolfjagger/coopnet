#pragma once

#include <deque>



namespace sat {

	using node_id = int;

	struct node {

		int id;

		explicit node(node_id i);

	};

	bool operator<(const node& node_1, const node& node_2);



	class nodelist {

	public:

		using storage = std::deque<node>;
		using iterator = storage::iterator;
		using const_iterator = storage::const_iterator;

	private:

		storage nodes;

	public:
		
		nodelist();

		template<typename iter>
		explicit nodelist(iter begin, iter end);


		// Init node and add to list
		void emplace(node_id&& init);
		void sort();

	private:

		// Push node to list
		void push(node&& node);

	public:

		// Give access to node iteration
		iterator begin() { return nodes.begin(); }
		const_iterator begin() const { return nodes.begin(); }
		const_iterator cbegin() const { return nodes.cbegin(); }
		iterator end() { return nodes.end(); }
		const_iterator end() const { return nodes.end(); }
		const_iterator cend() const { return nodes.cend(); }
		
	};

}
