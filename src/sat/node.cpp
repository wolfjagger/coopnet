#include "node.h"
#include "algorithm"

using namespace sat;



node::node(node_id i) {
	id = i;
}



bool sat::operator<(const node& node_1, const node& node_2) {
	return node_1.id < node_2.id;
}



nodelist::nodelist() {}

template<typename iter>
nodelist::nodelist(iter begin, iter end)
	: nodelist() {
	for (auto it = begin; it < end; ++it) {
		emplace(*it);
	}
}


void nodelist::emplace(node_id&& node_init) {
	push(node(node_init));
}

void nodelist::sort() {
	std::sort(nodes.begin(), nodes.end());
}


void nodelist::push(node&& node) {
	
	nodes.push_back(std::move(node));

}
