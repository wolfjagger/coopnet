#pragma once

#include <deque>



namespace sat {

	struct node {

		unsigned id;

		node(unsigned init);

	};

	bool operator<(const node& node_1, const node& node_2);
	bool operator==(const node& node_1, const node& node_2);



	class node_list {

	public:

		using storage = std::deque<node>;
		using iterator = storage::iterator;
		using const_iterator = storage::const_iterator;

	private:

		storage nodes;

	public:
		
		node_list();

		template<typename iter>
		explicit node_list(iter begin, iter end)
			: node_list() {
			for (auto it = begin; it != end; ++it) {
				emplace(*it);
			}
		}


		// Init node and add to list
		void emplace(int init);
		void sort();
		size_t size() const;

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
