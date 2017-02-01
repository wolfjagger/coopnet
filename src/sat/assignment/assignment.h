#pragma once

#include "sat/graph.h"
#include "boost/logic/tribool.hpp"



namespace sat {

	class problem;

	struct incomplete_assignment {

		using map = std::map<vertex_descriptor, boost::tribool>;
		using pair = std::pair<vertex_descriptor, boost::tribool>;

		map data;

		explicit incomplete_assignment(const problem& prob);
		incomplete_assignment() = default;

	};

	struct assignment {

		using map = std::map<vertex_descriptor, bool>;
		
		map data;

		assignment() = default;

		assignment(const incomplete_assignment& incomplete_assign);

	};

}
