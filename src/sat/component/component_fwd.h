#pragma once

#include <deque>



namespace sat {
	
	struct node;
	using node_list = std::deque<node>;

	struct clause;
	struct clause_data;
	using clause_list = std::deque<clause>;

}