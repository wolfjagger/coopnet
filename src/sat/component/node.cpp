#include "node.h"
#include "algorithm"

using namespace sat;



node::node(unsigned int init) {
	id = init;
}



bool sat::operator<(const node& node_1, const node& node_2) {
	return node_1.id < node_2.id;
}
bool sat::operator==(const node& node_1, const node& node_2) {
	return node_1.id == node_2.id;
}



node_list sat::create_nodes(size_t num_nodes) {
	auto list = node_list();
	for (auto i = 0; i < num_nodes; ++i) list.emplace_back(i);
	return list;
}
