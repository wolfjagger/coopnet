#include "assignment.h"
#include <algorithm>
#include "problem.h"

using namespace sat;



Assignment::Assignment(const IncompleteAssignment& incomplete_assign) {

	if (incomplete_assign.is_indeterminate()) {
		throw std::exception("Incomplete assignment cannot be transformed.");
	}

	auto copy_pred = [this, &inc_data = incomplete_assign.data](auto pair) {
		data.emplace(pair.first, bool(inc_data.at(pair.second)));
	};

	std::for_each(
		incomplete_assign.node_to_vertex_map->left.begin(),
		incomplete_assign.node_to_vertex_map->left.end(),
		copy_pred);

}



IncompleteAssignment::IncompleteAssignment(const Problem& prob) {

	node_to_vertex_map = prob.get_node_vert_map();
	std::for_each(node_to_vertex_map->left.begin(), node_to_vertex_map->left.end(),
		[this](auto& pair) { data.emplace(pair.second, boost::indeterminate); });

}

bool IncompleteAssignment::is_indeterminate() const {

	return std::any_of(data.cbegin(), data.cend(), is_ind_pair);

}
