#pragma once

#include "boost/logic/tribool.hpp"
#include "alphali/util/operators.h"
#include "coopnet/graph/graph.h"
#include "coopnet/sat/component/node.h"



namespace sat {

	class Problem;
	struct IncompleteAssignment;

	struct Assignment {

		using Map = std::map<Node, bool>;
		using Pair = std::pair<Node, bool>;
		
		Map data;

		Assignment() = default;
		Assignment(const IncompleteAssignment& incomplete_assign);

	};

	inline bool operator==(const Assignment& assign1, const Assignment& assign2) {
		return assign1.data == assign2.data;
	}

	inline bool operator<(const Assignment& assign1, const Assignment& assign2) {
		return assign1.data < assign2.data;
	}

	DEFINE_EXTRA_OPS(Assignment);



	struct IncompleteAssignment {

		using Map = std::map<VertDescriptor, boost::tribool>;
		using Pair = std::pair<VertDescriptor, boost::tribool>;

		Map data;
		std::shared_ptr<const NodeVertMap> node_to_vertex_map;

		explicit IncompleteAssignment(const Problem& prob);
		IncompleteAssignment() = default;

		bool is_indeterminate() const;

	};

	inline bool is_ind_assignment(const IncompleteAssignment& assign) {
		return assign.is_indeterminate();
	}

	inline bool is_ind_pair(const IncompleteAssignment::Pair& pair) {
		return boost::indeterminate(pair.second);
	}

}
