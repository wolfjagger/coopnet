#include "prune_info.h"

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



PruneAction::PruneAction(VertStatusData pruneData) :
	type(PruneObject::Vertex),
	suppData(pruneData) {

}

PruneAction::PruneAction(EdgeStatusData pruneData) :
	type(PruneObject::Edge),
	suppData(pruneData) {

}

PruneAction::PruneAction(IncompleteAssignmentData pruneData) :
	type(PruneObject::Assignment),
	suppData(pruneData) {

}





PruneStatus PruneInfo::get_vert_status(VertDescriptor v) const {
	return vertStatus.at(v);
}
void PruneInfo::set_vert_status(VertDescriptor v, PruneStatus newStatus) {

	auto& oldStatus = vertStatus.at(v);
	if (oldStatus != newStatus) {
		pruneStack.emplace(std::make_pair(v, oldStatus));
		oldStatus = newStatus;
	}

}

PruneStatus PruneInfo::get_edge_status(EdgeDescriptor e) const {
	return edgeStatus.at(e);
}
void PruneInfo::set_edge_status(EdgeDescriptor e, PruneStatus newStatus) {

	auto& oldStatus = edgeStatus.at(e);
	if (oldStatus != newStatus) {
		pruneStack.emplace(std::make_pair(e, oldStatus));
		oldStatus = newStatus;
	}

}

bool PruneInfo::is_indeterminate() const {

	return std::any_of(assignment.cbegin(), assignment.cend(),
		[](auto pair) { return boost::indeterminate(pair.second); });

}
boost::tribool PruneInfo::get_assignment(VertDescriptor v) const {
	return assignment.at(v);
}
void PruneInfo::set_assignment(VertDescriptor v, boost::tribool newValue) {

	auto& oldValue = assignment.at(v);
	if (oldValue != newValue) {
		pruneStack.emplace(std::make_pair(v, oldValue));
		oldValue = newValue;
	}

}

const IncompleteAssignmentMap& PruneInfo::get_assignment_map() const {
	return assignment;
}




void PruneInfo::reverse_to_vert(VertDescriptor v) {

	auto done = false;
	while (!done && !pruneStack.empty()) {

		auto action = pruneStack.top();
		pruneStack.pop();

		using prune_object = PruneAction::PruneObject;
		switch (action.type) {
		case prune_object::Vertex: {

			auto& vertexData =
				boost::get<VertStatusData>(action.suppData);
			auto vert = vertexData.first;
			auto status = vertexData.second;

			if(DEBUG)
				std::cout << "Prune vert status " << vert << " " << status << "\n";

			vertStatus[vert] = status;

			// If vert is prune-to vert, set to done (status is set first, not assignment)
			if (vert == v && status == PruneStatus::Active) done = true;

			break;

		}
		case prune_object::Edge: {

			auto& edgeData =
				boost::get<EdgeStatusData>(action.suppData);
			auto edge = edgeData.first;
			auto status = edgeData.second;

			if(DEBUG)
				std::cout << "Prune edge status " << edge << " " << status << "\n";

			edgeStatus[edge] = status;
			break;

		}
		case prune_object::Assignment: {

			auto& incompleteAssignmentData =
				boost::get<IncompleteAssignmentData>(action.suppData);
			auto vert = incompleteAssignmentData.first;
			auto value = incompleteAssignmentData.second;

			if (DEBUG)
				std::cout << "Prune assignment " << vert << " " << value << "\n";

			assignment[vert] = value;
			break;

		}}

	}

}

void PruneInfo::reset_prune() {

	std::for_each(vertStatus.begin(), vertStatus.end(), [](auto& pair) {
		pair.second = PruneStatus::Active;
	});

	std::for_each(edgeStatus.begin(), edgeStatus.end(), [](auto& pair) {
		pair.second = PruneStatus::Active;
	});

	std::for_each(assignment.begin(), assignment.end(), [](auto& pair) {
		pair.second = boost::indeterminate;
	});

	pruneStack = PruneStack();

}





std::ostream& sat::operator<<(std::ostream& os, PruneStatus status) {
	switch (status) {
	case PruneStatus::Active:
		os << "Active";
	case PruneStatus::Inactive:
		os << "Inactive";
	}
	return os;
}

std::ostream& sat::operator<<(std::ostream& os, boost::logic::tribool b) {
	if (b) {
		return os << "T";
	} else if (!b) {
		return os << "F";
	} else {
		return os << "I";
	}
}
