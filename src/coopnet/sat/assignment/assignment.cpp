#include "assignment.h"
#include <algorithm>
#include "coopnet/sat/problem.h"

using namespace sat;



incomplete_assignment::incomplete_assignment(const problem& prob) {

	const auto& map_node_to_vert = prob.get_map_node_to_vert();
	std::for_each(map_node_to_vert.cbegin(), map_node_to_vert.cend(),
		[this](auto& pair) { data.emplace(pair.second, boost::indeterminate); });

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

	auto copy_pred = [this](std::pair<vertex_descriptor, boost::tribool> pair) {
		data.emplace(pair.first, bool(pair.second));
	};

	std::for_each(
		incomplete_assign.data.cbegin(), incomplete_assign.data.cend(),
		copy_pred);

}
