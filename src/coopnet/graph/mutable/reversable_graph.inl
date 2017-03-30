


template<typename VProp, typename EProp>
ReversableSatGraph<VProp, EProp>::ReversableSatGraph(const BaseSatGraph& original) :
	graph(create_reversable_from_base(original)),
	reverseStack() {

	connectedComponentEntryPts
		= graph_util::calculate_connected_components(original);
	numConnectedComponents = connectedComponentEntryPts.size();

}





template<typename VProp, typename EProp>
PruneStatus ReversableSatGraph<VProp, EProp>::get_vert_status(VertDescriptor v) const {
	return graph[v].pruneStatus;
}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::set_vert_status(VertDescriptor v, PruneStatus newStatus) {

	auto& oldStatus = graph[v].pruneStatus;
	if (oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for vert " << v << " to " << newStatus << "\n";

		reverseStack.emplace(std::make_pair(v, oldStatus));
		oldStatus = newStatus;

	}

}

template<typename VProp, typename EProp>
PruneStatus ReversableSatGraph<VProp, EProp>::get_edge_status(EdgeDescriptor e) const {
	return graph[e].pruneStatus;
}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::set_edge_status(EdgeDescriptor e, PruneStatus newStatus) {

	auto& oldStatus = graph[e].pruneStatus;
	if (oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for edge " << e << " to " << newStatus << "\n";

		reverseStack.emplace(std::make_pair(e, oldStatus));
		oldStatus = newStatus;

	}

}

template<typename VProp, typename EProp>
boost::tribool ReversableSatGraph<VProp, EProp>::get_assignment(VertDescriptor v) const {
	return graph[v].assignment;
}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::set_assignment(VertDescriptor v, boost::tribool newValue) {

	auto& oldValue = graph[v].assignment;
	if (oldValue != newValue ||
		(!boost::indeterminate(oldValue) && boost::indeterminate(newValue)) ||
		(boost::indeterminate(oldValue) && !boost::indeterminate(newValue))) {

		if (DEBUG) std::cout << "Set assignment for vert " << v << " to " << newValue << "\n";

		reverseStack.emplace(std::make_pair(v, oldValue));
		oldValue = newValue;

	}

}

template<typename VProp, typename EProp>
bool ReversableSatGraph<VProp, EProp>::is_indeterminate_node(VertDescriptor v) const {

	return (graph[v].base.kind == PruneSatVProp::Node)
		&& (boost::indeterminate(graph[v].assignment));

}

template<typename VProp, typename EProp>
bool ReversableSatGraph<VProp, EProp>::is_indeterminate() const {

	auto vPair = boost::vertices(graph);
	return std::any_of(vPair.first, vPair.second, [this](VertDescriptor v) {
		return is_indeterminate_node(v);
	});

}



template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::reverse_to_vert(VertDescriptor v) {

	auto done = false;
	while (!done && !reverseStack.empty()) {

		auto& action = reverseStack.top();

		using reverseObject = ReversableAction::ReverseCategory;
		switch (action.type) {
		case reverseObject::Vertex: {

			auto& vertexData =
				boost::get<VertStatusPair>(action.suppData);
			auto vert = vertexData.first;
			auto status = vertexData.second;

			if (DEBUG)
				std::cout << "Prune vert status " << vert << " " << status << "\n";

			graph[vert].pruneStatus = status;

			// If vert is prune-to vert, set to done (status is set first, not assignment)
			if (vert == v && status == PruneStatus::Active) done = true;

			break;

		}
		case reverseObject::Edge: {

			auto& edgeData =
				boost::get<EdgeStatusPair>(action.suppData);
			auto edge = edgeData.first;
			auto status = edgeData.second;

			if (DEBUG)
				std::cout << "Prune edge status " << edge << " " << status << "\n";

			graph[edge].pruneStatus = status;
			break;

		}
		case reverseObject::Assignment: {

			auto& incompleteAssignmentData =
				boost::get<IncompleteAssignmentPair>(action.suppData);
			auto vert = incompleteAssignmentData.first;
			auto value = incompleteAssignmentData.second;

			if (DEBUG)
				std::cout << "Prune assignment " << vert << " " << value << "\n";

			graph[vert].assignment = value;
			break;

		}
		}

		reverseStack.pop();

	}

}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::reset_all() {

	auto vPair = boost::vertices(graph);
	auto ePair = boost::edges(graph);

	std::for_each(vPair.first, vPair.second, [this](VertDescriptor v) {
		graph[v].pruneStatus = PruneStatus::Active;
		graph[v].assignment = boost::indeterminate;
	});

	std::for_each(ePair.first, ePair.second, [this](EdgeDescriptor e) {
		graph[e].pruneStatus = PruneStatus::Active;
	});

	reverseStack = ReverseStack();

}





template<typename VProp, typename EProp>
size_t ReversableSatGraph<VProp, EProp>::num_connected_components() const {
	return numConnectedComponents;
}

template<typename VProp, typename EProp>
const std::vector<VertDescriptor>&
ReversableSatGraph<VProp, EProp>::connected_component_entry_pts() const {
	return connectedComponentEntryPts;
}



template<typename VProp, typename EProp>
template<typename PruneVisitor>
void ReversableSatGraph<VProp, EProp>::visit(PruneVisitor& v) {

	visit_sat_graph(graph, v,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}

template<typename VProp, typename EProp>
template<typename PruneVisitor>
void ReversableSatGraph<VProp, EProp>::visit(PruneVisitor& v) const {

	visit_sat_graph(graph, v,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}






template<typename VProp, typename EProp>
SatGraph<VProp, EProp> ReversableSatGraph<VProp, EProp>::create_reversable_from_base(
	const BaseSatGraph& original) {

	auto out = SatGraph<VProp, EProp>();

	auto vPair = boost::vertices(original);
	auto ePair = boost::edges(original);

	auto mapOrigToOut = std::map<VertDescriptor, VertDescriptor>();

	std::for_each(vPair.first, vPair.second,
		[&out, &original, &mapOrigToOut](VertDescriptor v) {

		auto prop = VProp();
		prop.base = original[v].base;

		auto outVert = boost::add_vertex(prop, out);
		mapOrigToOut.emplace(v, outVert);

	});

	std::for_each(ePair.first, ePair.second,
		[&out, &original, &mapOrigToOut](EdgeDescriptor e) {

		auto prop = EProp();
		prop.base = original[e].base;

		auto vert1 = boost::source(e, out);
		auto vert2 = boost::target(e, out);

		boost::add_edge(vert1, vert2, prop, out);

	});

	return out;

}
