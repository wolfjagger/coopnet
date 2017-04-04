#pragma once

#include <map>
#include "alphali/util/operators.h"
#include "alphali/util/random.h"
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



	template<typename NodeIterator>
	Assignment rand_assignment(NodeIterator nodeBegin, NodeIterator nodeEnd) {

		Assignment assignment;
		for (auto node = nodeBegin; node != nodeEnd; ++node) {
			assignment.data.emplace(*node, alphali::coin_flip());
		}

		return assignment;

	}



	inline std::ostream& operator<<(
		std::ostream& os, const coopnet::Assignment& assign) {
		os << "assignment:" << std::endl;
		for (auto iter = assign.data.cbegin();
			iter != assign.data.cend(); ++iter) {
			os << " k" << iter->first.id << " v" << iter->second ? " T" : " F";
		}
		return os << std::endl;
	}

}
