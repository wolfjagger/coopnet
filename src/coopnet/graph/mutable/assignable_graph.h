#pragma once

#include "coopnet/graph/graph.h"
#include "boost/logic/tribool.hpp"



namespace coopnet {

	struct AssignSatVProp : public virtual BaseSatVProp {
		mutable boost::tribool assignment;

		AssignSatVProp() :
			BaseSatVProp(),
			assignment(boost::indeterminate) {}

	};

	struct AssignSatEProp : public virtual BaseSatEProp { };

	using AssignSatGraph = SatGraph<AssignSatVProp, AssignSatEProp>;


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
