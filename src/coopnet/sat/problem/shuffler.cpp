#include "shuffler.h"
#include <forward_list>
#include <numeric>
#include "alphali/util/random.h"
#include "problem.h"
#include "assignment.h"
#include "coopnet/graph/graph_util.h"

using namespace coopnet;



// Note: shuffles on creation
NodeShuffler::NodeShuffler(const Problem& prob) {

	nodes = std::vector<Node>();
	auto num_nodes = prob.get_num_nodes();
	nodes.reserve(num_nodes);
	for (unsigned int i = 0; i < num_nodes; ++i) {
		nodes.emplace_back(i);
	}

	map_node_to_vert = prob.get_node_vert_map();

	shuffle();

}

NodeShuffler::NodeShuffler(const Problem& prob, std::vector<Node> shuffle_nodes) {

	nodes = std::move(shuffle_nodes);
	map_node_to_vert = prob.get_node_vert_map();

}



void NodeShuffler::shuffle() {
	alphali::shuffle(nodes.begin(), nodes.end());
}


bool NodeShuffler::is_trivial() const {
	return std::is_sorted(nodes.begin(), nodes.end());
}


Node NodeShuffler::shuffled_node(Node orig) const {
	return nodes.at(orig.id);
}


void NodeShuffler::apply_to_assignment(Assignment& assign) const {

	auto old_map = assign.data;
	for(auto& n : nodes) {
		assign.data.at(n) = old_map.at(n);
	}

}

void NodeShuffler::apply_to_problem(Problem& prob) const {

	// Change node_vert_map to re-point the nodes
	// Need copy and swap because we can't have duplicates
	auto map_cpy = NodeVertMap();
	for (auto iter = prob.map_node_to_vert->left.begin();
		iter != prob.map_node_to_vert->left.end(); ++iter) {

		auto old_node = iter->first;
		auto vert = iter->second;
		auto new_node = nodes.at(old_node.id);
		map_cpy.left.insert(std::make_pair(new_node, vert));

	}
	std::swap(map_cpy, *prob.map_node_to_vert);

	// Rename nodes and clauses
	graph_util::rename_verts(prob.prob_graph, *map_node_to_vert);

}



SgnShuffler::SgnShuffler(const Problem& prob) {

	sgns = std::vector<bool>();
	auto num_nodes = prob.get_num_nodes();
	sgns.reserve(num_nodes);
	for (size_t i = 0; i < num_nodes; ++i) {
		sgns.emplace_back(true);
	}

	map_node_to_vert = prob.get_node_vert_map();

	shuffle();

}

SgnShuffler::SgnShuffler(const Problem& prob, std::vector<bool> shuffle_sgns) {

	sgns = std::move(shuffle_sgns);
	map_node_to_vert = prob.get_node_vert_map();

}



void SgnShuffler::shuffle() {

	for (auto& sgn : sgns) {
		sgn = alphali::coin_flip();
	}

}


bool SgnShuffler::is_trivial() const {
	return std::all_of(sgns.begin(), sgns.end(),
		[](bool b) { return b == true;});
}


bool SgnShuffler::shuffled_sgn(Node n) const {
	return sgns.at(n.id);
}


void SgnShuffler::apply_to_assignment(Assignment& assign) const {

	for (unsigned int i = 0; i < sgns.size(); ++i) {
		if (!sgns[i]) {
			auto& sgn = assign.data.at(Node(i));
			sgn = !sgn;
		}
	}

}

void SgnShuffler::apply_to_problem(Problem& prob) const {

	// Change edge sgns

	auto& prob_graph = prob.prob_graph;

	for (auto iter = prob.map_node_to_vert->left.begin();
		iter != prob.map_node_to_vert->left.end(); ++iter) {

		if (!sgns.at(iter->first.id)) {

			auto vert = iter->second;

			auto edge_pair = boost::out_edges(vert, prob_graph);
			for_each(edge_pair.first, edge_pair.second, [&prob_graph](EdgeDescriptor e) {
				prob_graph[e].base.sgn = !prob_graph[e].base.sgn;
			});

		}
	}

	// Rename nodes and clauses
	graph_util::rename_verts(prob_graph, *map_node_to_vert);

}



LiteralShuffler::LiteralShuffler(const Problem& prob) :
	node_sh(prob),
	sgn_sh(prob) {

}

namespace {
	std::vector<Node> nodes_from_lit(const std::vector<Literal>& lits) {
		auto nodes = std::vector<Node>();
		std::for_each(lits.cbegin(), lits.cend(),
			[&nodes](Literal lit) { nodes.push_back(lit.n); });
		return nodes;
	}
	std::vector<bool> sgns_from_lit(const std::vector<Literal>& lits) {
		auto sgns = std::vector<bool>();
		std::for_each(lits.cbegin(), lits.cend(),
			[&sgns](Literal lit) { sgns.push_back(lit.sgn); });
		return sgns;
	}
}

LiteralShuffler::LiteralShuffler(
	const Problem& prob, const std::vector<Literal>& lits) :
	node_sh(prob, nodes_from_lit(lits)),
	sgn_sh(prob, sgns_from_lit(lits)) {

}



void LiteralShuffler::shuffle() {

	node_sh.shuffle();
	sgn_sh.shuffle();

}



bool LiteralShuffler::is_trivial() const {
	return !(shuffles_nodes() || flips_sgns());
}

bool LiteralShuffler::shuffles_nodes() const {
	return !node_sh.is_trivial();
}

bool LiteralShuffler::flips_sgns() const {
	return !sgn_sh.is_trivial();
}



Literal LiteralShuffler::shuffled_literal(Literal lit) const {

	lit.n = node_sh.shuffled_node(lit.n);
	lit.sgn = sgn_sh.shuffled_sgn(lit.n);
	return lit;

}

void LiteralShuffler::apply_to_assignment(Assignment& assign) const {

	node_sh.apply_to_assignment(assign);
	sgn_sh.apply_to_assignment(assign);

}

void LiteralShuffler::apply_to_problem(Problem& prob) const {

	node_sh.apply_to_problem(prob);
	sgn_sh.apply_to_problem(prob);

}
