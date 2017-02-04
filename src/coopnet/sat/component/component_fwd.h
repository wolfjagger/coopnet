#pragma once

#include <deque>



namespace sat {
	
	struct node;
	using node_list = std::deque<node>;
	struct literal;

	struct clause;
	using clause_list = std::deque<clause>;

}