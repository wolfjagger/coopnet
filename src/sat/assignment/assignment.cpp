#include "assignment.h"
#include "sat/problem.h"

using namespace sat;



incomplete_assignment::incomplete_assignment(const problem& prob) {

	const auto& map_node_to_vert = prob.get_map_node_to_vert();
	std::for_each(map_node_to_vert.cbegin(), map_node_to_vert.cend(),
		[this](auto& pair) { data.emplace(pair.second, boost::indeterminate); });

}
