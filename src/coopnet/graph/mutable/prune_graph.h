#pragma once

#include "coopnet/graph/base/graph.h"



namespace coopnet {

	enum class PruneStatus {
		Active, Inactive
	};


	struct PruneSatProp {

		struct Node : public virtual BaseSatProp::Node {
			mutable PruneStatus pruneStatus;

			Node() :
				BaseSatProp::Node(),
				pruneStatus(PruneStatus::Active) {}

		};

		struct Clause : public virtual BaseSatProp::Clause {
			mutable PruneStatus pruneStatus;

			Clause() :
				BaseSatProp::Clause(),
				pruneStatus(PruneStatus::Active) {}

		};

		struct Edge : public virtual BaseSatProp::Edge {
			mutable PruneStatus pruneStatus;

			Edge() :
				BaseSatProp::Edge(),
				pruneStatus(PruneStatus::Active) {}

		};

	};


	using PruneSatGraph = SatGraph<PruneSatProp>;


	inline std::ostream& operator<<(std::ostream& os, PruneStatus status) {
		switch (status) {
		case PruneStatus::Active:
			os << "Active";
		case PruneStatus::Inactive:
			os << "Inactive";
		}
		return os;
	}

}
