#pragma once

#include "coopnet/graph/base/graph.h"
#include "boost/logic/tribool.hpp"



namespace coopnet {

	struct AssignSatProp {

		struct Node : public virtual BaseSatProp::Node {
			mutable boost::tribool assignment;

			Node() :
				BaseSatProp::Node(),
				assignment(boost::indeterminate) {}

		};

		struct Clause : public virtual BaseSatProp::Clause { };

		struct Edge : public virtual BaseSatProp::Edge { };

	};

	using AssignSatGraph = SatGraph<AssignSatProp>;


	inline std::ostream& operator<<(std::ostream& os, boost::logic::tribool b) {
		if (b) {
			return os << "T";
		} else if (!b) {
			return os << "F";
		} else {
			return os << "I";
		}
	}

}
