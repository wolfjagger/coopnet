#include "assignment.h"
#include <algorithm>
#include "problem.h"

using namespace sat;



incomplete_assignment::incomplete_assignment(const problem& prob) {

	const auto& map_node_to_vert = prob.get_node_vert_map();
	std::for_each(map_node_to_vert.left.begin(), map_node_to_vert.left.end(),
		[this](auto& pair) { data.emplace(pair.first, boost::indeterminate); });

}

bool incomplete_assignment::is_indeterminate() const {

	return std::any_of(data.cbegin(), data.cend(), [](const pair& p) {
		return boost::indeterminate(p.second);
	});

}



assignment::assignment(const incomplete_assignment& incomplete_assign) {

	if (incomplete_assign.is_indeterminate()) {
		throw std::exception("Incomplete assignment cannot be transformed.");
	}

	auto copy_pred = [this](std::pair<node, boost::tribool> pair) {
		data.emplace(pair.first, bool(pair.second));
	};

	std::for_each(
		incomplete_assign.data.cbegin(), incomplete_assign.data.cend(),
		copy_pred);

}
