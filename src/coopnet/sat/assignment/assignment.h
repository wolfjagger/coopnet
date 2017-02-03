#pragma once

#include "boost/logic/tribool.hpp"
#include "coopnet/graph/graph.h"



namespace sat {

	class problem;

	struct incomplete_assignment {

		using map = std::map<vertex_descriptor, boost::tribool>;
		using pair = std::pair<vertex_descriptor, boost::tribool>;

		map data;

		explicit incomplete_assignment(const problem& prob);
		incomplete_assignment() = default;

		bool is_indeterminate() const;

	};

	inline bool is_indeterminate(const incomplete_assignment& assign) {
		return assign.is_indeterminate();
	}


	struct assignment {

		using map = std::map<vertex_descriptor, bool>;
		
		map data;

		assignment() = default;
		assignment(const incomplete_assignment& incomplete_assign);

	};

}
