#pragma once

#include "boost/logic/tribool.hpp"
#include "alphali/util/operators.h"
#include "coopnet/graph/graph.h"
#include "coopnet/sat/component/node.h"



namespace sat {

	class problem;
	struct incomplete_assignment;

	struct assignment {

		using map = std::map<node, bool>;
		using pair = std::pair<node, bool>;
		
		map data;

		assignment() = default;
		assignment(const incomplete_assignment& incomplete_assign);

	};

	inline bool operator==(const assignment& assign1, const assignment& assign2) {
		return assign1.data == assign2.data;
	}

	inline bool operator<(const assignment& assign1, const assignment& assign2) {
		return assign1.data < assign2.data;
	}

	DEFINE_EXTRA_OPS(assignment);



	struct incomplete_assignment {

		using map = std::map<vertex_descriptor, boost::tribool>;
		using pair = std::pair<vertex_descriptor, boost::tribool>;

		map data;
		std::shared_ptr<const node_vert_map> node_to_vertex_map;

		explicit incomplete_assignment(const problem& prob);
		incomplete_assignment() = default;

		bool is_indeterminate() const;

	};

	inline bool is_ind_assignment(const incomplete_assignment& assign) {
		return assign.is_indeterminate();
	}

	inline bool is_ind_pair(const incomplete_assignment::pair& pair) {
		return boost::indeterminate(pair.second);
	}

}
