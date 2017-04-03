#include "sat_graph_translator.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/component/node.h"

using namespace coopnet;



SatGraphTranslator::SatGraphTranslator(const NodeVertMap& initNodeVertMap) :
	nodeVertMap(initNodeVertMap) {

	init_node_list();

}

SatGraphTranslator::SatGraphTranslator(NodeVertMap&& initNodeVertMap) :
	nodeVertMap(std::move(initNodeVertMap)) {

	init_node_list();

}

SatGraphTranslator::SatGraphTranslator(const Problem& prob) :
	nodeVertMap(prob.get_node_vert_translator()->nodeVertMap) {

	init_node_list();

}



VertDescriptor SatGraphTranslator::node_to_vert(Node node) const {
	return nodeVertMap.left.at(node);
}

Node SatGraphTranslator::vert_to_node(VertDescriptor vert) const {
	return nodeVertMap.right.at(vert);
}



void SatGraphTranslator::init_node_list() {

	nodeList = NodeList();
	for (auto pair = nodeVertMap.left.begin(); pair != nodeVertMap.left.end(); ++pair) {
		nodeList.emplace_back(pair->first);
	}

}
