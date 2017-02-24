#pragma once

#include <map>
#include "alphali/util/operators.h"
#include "coopnet/sat/component/node.h"



namespace coopnet {

	struct Assignment {

		using Map = std::map<Node, bool>;
		using Pair = std::pair<Node, bool>;
		
		Map data;

		Assignment() = default;

	};

	inline bool operator==(const Assignment& assign1, const Assignment& assign2) {
		return assign1.data == assign2.data;
	}

	inline bool operator<(const Assignment& assign1, const Assignment& assign2) {
		return assign1.data < assign2.data;
	}

	DEFINE_EXTRA_OPS(Assignment);

}
