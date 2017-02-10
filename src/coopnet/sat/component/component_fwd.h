#pragma once

#include <deque>
#include <set>



namespace sat {
	
	struct node;
	using node_list = std::deque<node>;
	struct literal;
	
	struct clause;
	using clause_list = std::set<clause>;

}