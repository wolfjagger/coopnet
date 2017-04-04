


template<typename VProp, typename EProp>
ReversableSatGraph<VProp, EProp>::ReversableSatGraph(
	const BaseSatGraph& original, const SatGraphTranslator& origTranslator) :
	graph(graph_util::create_default_concrete_graph<VProp, EProp>(
		original, origTranslator)),
	reverseStack() {

	connectedComponentEntryPts
		= graph_util::calculate_connected_components(graph.graph);
	numConnectedComponents = connectedComponentEntryPts.size();

}





template<typename VProp, typename EProp>
PruneStatus ReversableSatGraph<VProp, EProp>::get_vert_status(VertDescriptor v) const {
	return graph.graph[v].pruneStatus;
}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::set_vert_status(VertDescriptor v, PruneStatus newStatus) {

	auto& oldStatus = graph.graph[v].pruneStatus;
	if (oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for vert " << v << " to " << newStatus << "\n";

		reverseStack.emplace(std::make_pair(v, oldStatus));
		oldStatus = newStatus;

	}

}

template<typename VProp, typename EProp>
PruneStatus ReversableSatGraph<VProp, EProp>::get_edge_status(EdgeDescriptor e) const {
	return graph.graph[e].pruneStatus;
}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::set_edge_status(EdgeDescriptor e, PruneStatus newStatus) {

	auto& oldStatus = graph.graph[e].pruneStatus;
	if (oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for edge " << e << " to " << newStatus << "\n";

		reverseStack.emplace(std::make_pair(e, oldStatus));
		oldStatus = newStatus;

	}

}

template<typename VProp, typename EProp>
boost::tribool ReversableSatGraph<VProp, EProp>::get_assignment(VertDescriptor v) const {
	return graph.graph[v].assignment;
}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::set_assignment(VertDescriptor v, boost::tribool newValue) {

	auto& oldValue = graph.graph[v].assignment;
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

	return (graph.graph[v].base.kind == PruneSatVProp::Node)
		&& (boost::indeterminate(graph.graph[v].assignment));

}

template<typename VProp, typename EProp>
bool ReversableSatGraph<VProp, EProp>::is_indeterminate() const {

	auto vPair = boost::vertices(graph.graph);
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

			graph.graph[vert].pruneStatus = status;

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

			graph.graph[vert].assignment = value;
			break;

		}
		}

		reverseStack.pop();

	}

}

template<typename VProp, typename EProp>
void ReversableSatGraph<VProp, EProp>::reset_all() {

	auto vPair = boost::vertices(graph.graph);
	auto ePair = boost::edges(graph.graph);

	std::for_each(vPair.first, vPair.second, [this](VertDescriptor v) {
		graph.graph[v].pruneStatus = PruneStatus::Active;
		graph.graph[v].assignment = boost::indeterminate;
	});

	std::for_each(ePair.first, ePair.second, [this](EdgeDescriptor e) {
		graph.graph[e].pruneStatus = PruneStatus::Active;
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

	visit_sat_graph(graph.graph, v,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}

template<typename VProp, typename EProp>
template<typename PruneVisitor>
void ReversableSatGraph<VProp, EProp>::visit(PruneVisitor& v) const {

	visit_sat_graph(graph.graph, v,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}
