#include "shuffler.h"
#include <forward_list>
#include <numeric>
#include "alphali/util/random.h"
#include "problem.h"
#include "assignment.h"

using namespace sat;



// Note: shuffles on creation
node_shuffler::node_shuffler(const problem& prob) {

	nodes = std::vector<node>();
	auto num_nodes = prob.get_num_nodes();
	nodes.reserve(num_nodes);
	for (auto i = 0; i < num_nodes; ++i) {
		nodes.emplace_back(i);
	}

	map_node_to_vert = prob.get_node_vert_map();

	shuffle();

}

node_shuffler::node_shuffler(const problem& prob, std::vector<node> shuffle_nodes) {

	nodes = std::move(shuffle_nodes);
	map_node_to_vert = prob.get_node_vert_map();

}



void node_shuffler::shuffle() {
	alphali::shuffle(nodes.begin(), nodes.end());
}


bool node_shuffler::is_trivial() const {
	return std::is_sorted(nodes.begin(), nodes.end());
}


node node_shuffler::shuffled_node(node orig) const {
	return nodes.at(orig.id);
}


void node_shuffler::apply_to_assignment(assignment& assign) const {

	auto old_map = assign.data;
	for (auto i = 0; i < nodes.size(); ++i) {
		assign.data.at(nodes[i]) = old_map.at(node(i));
	}

}

void node_shuffler::apply_to_problem(problem& prob) const {
	prob.shuffle_nodes(*this);
}



sgn_shuffler::sgn_shuffler(const problem& prob) {

	sgns = std::vector<bool>();
	auto num_nodes = prob.get_num_nodes();
	sgns.reserve(num_nodes);
	for (auto i = 0; i < num_nodes; ++i) {
		sgns.emplace_back(true);
	}

	map_node_to_vert = prob.get_node_vert_map();

	shuffle();

}

sgn_shuffler::sgn_shuffler(const problem& prob, std::vector<bool> shuffle_sgns) {

	sgns = std::move(shuffle_sgns);
	map_node_to_vert = prob.get_node_vert_map();

}



void sgn_shuffler::shuffle() {

	for (auto i = 0; i < sgns.size(); ++i) {
		sgns[i] = alphali::coin_flip();
	}

}


bool sgn_shuffler::is_trivial() const {
	return std::all_of(sgns.begin(), sgns.end(),
		[](bool b) { return b == true;});
}


bool sgn_shuffler::shuffled_sgn(node n) const {
	return sgns.at(n.id);
}


void sgn_shuffler::apply_to_assignment(assignment& assign) const {

	for (auto i = 0; i < sgns.size(); ++i) {
		if (!sgns[i]) {
			auto& sgn = assign.data.at(node(i));
			sgn = !sgn;
		}
	}

}

void sgn_shuffler::apply_to_problem(problem& prob) const {
	prob.shuffle_sgns(*this);
}



literal_shuffler::literal_shuffler(const problem& prob) :
	node_sh(prob),
	sgn_sh(prob) {

}

namespace {
	std::vector<node> nodes_from_lit(const std::vector<literal>& lits) {
		auto nodes = std::vector<node>();
		std::for_each(lits.cbegin(), lits.cend(),
			[&nodes](literal lit) { nodes.push_back(lit.n); });
		return nodes;
	}
	std::vector<bool> sgns_from_lit(const std::vector<literal>& lits) {
		auto sgns = std::vector<bool>();
		std::for_each(lits.cbegin(), lits.cend(),
			[&sgns](literal lit) { sgns.push_back(lit.sgn); });
		return sgns;
	}
}

literal_shuffler::literal_shuffler(
	const problem& prob, const std::vector<literal>& lits) :
	node_sh(prob, nodes_from_lit(lits)),
	sgn_sh(prob, sgns_from_lit(lits)) {

}



void literal_shuffler::shuffle() {

	node_sh.shuffle();
	sgn_sh.shuffle();

}



bool literal_shuffler::is_trivial() const {
	return !(shuffles_nodes() || flips_sgns());
}

bool literal_shuffler::shuffles_nodes() const {
	return !node_sh.is_trivial();
}

bool literal_shuffler::flips_sgns() const {
	return !sgn_sh.is_trivial();
}



literal literal_shuffler::shuffled_literal(literal lit) const {

	lit.n = node_sh.shuffled_node(lit.n);
	lit.sgn = sgn_sh.shuffled_sgn(lit.n);
	return lit;

}

void literal_shuffler::apply_to_assignment(assignment& assign) const {

	node_sh.apply_to_assignment(assign);
	sgn_sh.apply_to_assignment(assign);

}

void literal_shuffler::apply_to_problem(problem& prob) const {

	node_sh.apply_to_problem(prob);
	sgn_sh.apply_to_problem(prob);

}
