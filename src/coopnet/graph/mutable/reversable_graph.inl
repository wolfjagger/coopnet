


template<typename SatProp>
ReversableSatGraph<SatProp>::ReversableSatGraph(
	const BaseSatGraph& original, const SatGraphTranslator& origTranslator) :
	graph(graph_util::create_default_concrete_graph<SatProp>(
		original, origTranslator)),
	reverseStack() {

	connectedComponentEntryPts
		= graph_util::calculate_connected_components(graph.graph);
	numConnectedComponents = connectedComponentEntryPts.size();

}





template<typename SatProp>
PruneStatus ReversableSatGraph<SatProp>::get_vert_status(VertDescriptor v) const {
	auto& prop = graph.graph[vert];
	switch (prop.kind()) {
	case VertKind::Node:
		return prop.node().pruneStatus = status;
	case VertKind::Node:
		return prop.clause().pruneStatus = status;
	}
}

template<typename SatProp>
void ReversableSatGraph<SatProp>::set_vert_status(VertDescriptor v, PruneStatus newStatus) {

	PruneStatus* oldStatus;

	auto& prop = graph.graph[vert];
	switch (prop.kind()) {
	case VertKind::Node:
		oldStatus = &prop.node().pruneStatus;
		break;
	case VertKind::Node:
		oldStatus = &prop.clause().pruneStatus;
		break;
	}
	
	if (*oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for vert " << v << " to " << newStatus << "\n";

		reverseStack.emplace(std::make_pair(v, *oldStatus));
		*oldStatus = newStatus;

	}

}

template<typename SatProp>
PruneStatus ReversableSatGraph<SatProp>::get_edge_status(EdgeDescriptor e) const {
	return graph.graph[e].pruneStatus;
}

template<typename SatProp>
void ReversableSatGraph<SatProp>::set_edge_status(EdgeDescriptor e, PruneStatus newStatus) {

	auto& oldStatus = graph.graph[e].pruneStatus;
	if (oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for edge " << e << " to " << newStatus << "\n";

		reverseStack.emplace(std::make_pair(e, oldStatus));
		oldStatus = newStatus;

	}

}

template<typename SatProp>
boost::tribool ReversableSatGraph<SatProp>::get_assignment(VertDescriptor v) const {
	return graph.graph[v].node().assignment;
}

template<typename SatProp>
void ReversableSatGraph<SatProp>::set_assignment(VertDescriptor v, boost::tribool newValue) {

	auto& oldValue = graph.graph[v].node().assignment;
	if (oldValue != newValue ||
		(!boost::indeterminate(oldValue) && boost::indeterminate(newValue)) ||
		(boost::indeterminate(oldValue) && !boost::indeterminate(newValue))) {

		if (DEBUG) std::cout << "Set assignment for vert " << v << " to " << newValue << "\n";

		reverseStack.emplace(std::make_pair(v, oldValue));
		oldValue = newValue;

	}

}

template<typename SatProp>
bool ReversableSatGraph<SatProp>::is_indeterminate_node(VertDescriptor v) const {

	return (graph.graph[v].kind() == VertKind::Node)
		&& (boost::indeterminate(graph.graph[v].node().assignment));

}

template<typename SatProp>
bool ReversableSatGraph<SatProp>::is_indeterminate() const {

	auto vPair = boost::vertices(graph.graph);
	return std::any_of(vPair.first, vPair.second, [this](VertDescriptor v) {
		return is_indeterminate_node(v);
	});

}



template<typename SatProp>
void ReversableSatGraph<SatProp>::reverse_to_vert(VertDescriptor v) {

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

			auto& prop = graph.graph[vert];
			switch (prop.kind()) {
			case VertKind::Node:
				prop.node().pruneStatus = status;
				break;
			case VertKind::Clause:
				prop.clause().pruneStatus = status;
				break;
			}

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

			graph.graph[edge].pruneStatus = status;
			break;

		}
		case reverseObject::Assignment: {

			auto& incompleteAssignmentData =
				boost::get<IncompleteAssignmentPair>(action.suppData);
			auto vert = incompleteAssignmentData.first;
			auto value = incompleteAssignmentData.second;

			if (DEBUG)
				std::cout << "Prune assignment " << vert << " " << value << "\n";

			graph.graph[vert].node().assignment = value;
			break;

		}
		}

		reverseStack.pop();

	}

}

template<typename SatProp>
void ReversableSatGraph<SatProp>::reset_all() {

	auto vPair = boost::vertices(graph.graph);
	auto ePair = boost::edges(graph.graph);

	std::for_each(vPair.first, vPair.second, [this](VertDescriptor v) {
		auto& prop = graph.graph[v];
		switch(prop.kind()) {
		case VertKind::Node:
			prop.node().pruneStatus = PruneStatus::Active;
			prop.node().assignment = boost::indeterminate;
			break;
		case VertKind::Clause:
			prop.clause().pruneStatus = PruneStatus::Active;
			break;
		}
	});

	std::for_each(ePair.first, ePair.second, [this](EdgeDescriptor e) {
		graph.graph[e].pruneStatus = PruneStatus::Active;
	});

	reverseStack = ReverseStack();

}





template<typename SatProp>
size_t ReversableSatGraph<SatProp>::num_connected_components() const {
	return numConnectedComponents;
}

template<typename SatProp>
const std::vector<VertDescriptor>&
ReversableSatGraph<SatProp>::connected_component_entry_pts() const {
	return connectedComponentEntryPts;
}



template<typename SatProp>
template<typename PruneVisitor>
void ReversableSatGraph<SatProp>::visit(PruneVisitor& v) {

	visit_sat_graph(graph.graph, v,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}

template<typename SatProp>
template<typename PruneVisitor>
void ReversableSatGraph<SatProp>::visit(PruneVisitor& v) const {

	visit_sat_graph(graph.graph, v,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}
