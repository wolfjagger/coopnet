#include "node.h"
#include "algorithm"

using namespace sat;



node::node(unsigned init) {
	id = init;
}



bool sat::operator<(const node& node_1, const node& node_2) {
	return node_1.id < node_2.id;
}
bool sat::operator==(const node& node_1, const node& node_2) {
	return node_1.id == node_2.id;
}



node_list::node_list() {}



void node_list::emplace(int init) {
	push(node(init));
}

void node_list::sort() {
	std::sort(nodes.begin(), nodes.end());
}

size_t node_list::size() const {
	return nodes.size();
}


void node_list::push(node&& node) {
	
	nodes.push_back(std::move(node));

}
