#pragma once

#include "coopnet/graph/graph.h"



namespace coopnet {

	enum class PruneStatus {
		Active, Inactive
	};


	struct PruneSatVProp : public virtual BaseSatVProp {
		mutable PruneStatus pruneStatus;

		PruneSatVProp() :
			BaseSatVProp(),
			pruneStatus(PruneStatus::Active) {}

	};

	struct PruneSatEProp : public virtual BaseSatEProp {
		mutable PruneStatus pruneStatus;

		PruneSatEProp() :
			BaseSatEProp(),
			pruneStatus(PruneStatus::Active) {}

	};


	using PruneSatGraph = SatGraph<PruneSatVProp, PruneSatEProp>;


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
