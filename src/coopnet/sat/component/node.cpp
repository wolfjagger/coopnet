#include "node.h"

using namespace coopnet;



Node::Node(unsigned int init) {
	id = init;
}



bool coopnet::operator<(const Node& node1, const Node& node2) {
	return node1.id < node2.id;
}
bool coopnet::operator==(const Node& node1, const Node& node2) {
	return node1.id == node2.id;
}



NodeList coopnet::create_nodes(size_t num_nodes) {
	auto list = NodeList();
	for (unsigned int i = 0; i < num_nodes; ++i) list.emplace_back(i);
	return list;
}



Literal::Literal(Node n_init, bool sgn_init) :
	n(n_init),
	sgn(sgn_init) { }


bool coopnet::operator<(const Literal& lit1, const Literal& lit2) {
	if (lit1.n < lit2.n) return true;
	else return (!(lit1.n > lit2.n) && lit1.sgn < lit2.sgn);
}
bool coopnet::operator==(const Literal& lit1, const Literal& lit2) {
	return (lit1.n == lit2.n && lit1.sgn == lit2.sgn);
}
