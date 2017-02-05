#include "node.h"

using namespace sat;



node::node(unsigned int init) {
	id = init;
}



bool sat::operator<(const node& node1, const node& node2) {
	return node1.id < node2.id;
}
bool sat::operator==(const node& node1, const node& node2) {
	return node1.id == node2.id;
}



node_list sat::create_nodes(size_t num_nodes) {
	auto list = node_list();
	for (auto i = 0; i < num_nodes; ++i) list.emplace_back(i);
	return list;
}



literal::literal(node n_init, bool sgn_init) :
	n(n_init),
	sgn(sgn_init) { }


bool sat::operator<(const literal& lit1, const literal& lit2) {
	if (lit1.n < lit2.n) return true;
	else return (!(lit1.n > lit2.n) && lit1.sgn < lit2.sgn);
}
bool sat::operator==(const literal& lit1, const literal& lit2) {
	return (lit1.n == lit2.n && lit1.sgn == lit2.sgn);
}
