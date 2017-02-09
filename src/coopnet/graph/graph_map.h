#pragma once

#include "boost/bimap.hpp"
#include "graph.h"



namespace sat {

	struct node;

	using node_vert_map = boost::bimap<node, vertex_descriptor>;

}
