#pragma once

#include "sat/graph.h"
#include "boost/logic/tribool.hpp"



namespace sat {

	class problem;

	struct assignment {

		using map = std::map<vertex_descriptor, bool>;
		
		map data;

	};

	struct incomplete_assignment {

		using map = std::map<vertex_descriptor, boost::tribool>;

		map data;

		explicit incomplete_assignment(const problem& prob);
		incomplete_assignment() = default;

	};

}
