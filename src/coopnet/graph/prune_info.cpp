#include "prune_info.h"

using namespace coopnet;



PruneAction::PruneAction(VertStatusPair pruneData) :
	type(PruneObject::Vertex),
	suppData(pruneData) {

}

PruneAction::PruneAction(EdgeStatusPair pruneData) :
	type(PruneObject::Edge),
	suppData(pruneData) {

}

PruneAction::PruneAction(IncompleteAssignmentPair pruneData) :
	type(PruneObject::Assignment),
	suppData(pruneData) {

}





std::ostream& coopnet::operator<<(std::ostream& os, PruneStatus status) {
	switch (status) {
	case PruneStatus::Active:
		os << "Active";
	case PruneStatus::Inactive:
		os << "Inactive";
	}
	return os;
}

std::ostream& coopnet::operator<<(std::ostream& os, boost::logic::tribool b) {
	if (b) {
		return os << "T";
	} else if (!b) {
		return os << "F";
	} else {
		return os << "I";
	}
}
