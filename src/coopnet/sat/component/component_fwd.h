#pragma once

#include <deque>
#include <set>



namespace coopnet {
	
	struct Node;
	using NodeList = std::deque<Node>;
	struct Literal;
	
	struct Clause;
	using ClauseList = std::set<Clause>;

}