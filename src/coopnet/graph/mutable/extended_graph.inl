


template<typename VProp, typename EProp>
ExtendedSatGraph<VProp, EProp>::ExtendedSatGraph(const BaseSatGraph& original) :
	graph(create_extended_from_base(original)),
	pruneStack() {

	connectedComponentEntryPts
		= graph_util::calculate_connected_components(original);
	numConnectedComponents = connectedComponentEntryPts.size();

}





template<typename VProp, typename EProp>
PruneStatus ExtendedSatGraph<VProp, EProp>::get_vert_status(VertDescriptor v) const {
	return graph[v].mutate.status;
}

template<typename VProp, typename EProp>
void ExtendedSatGraph<VProp, EProp>::set_vert_status(VertDescriptor v, PruneStatus newStatus) {

	auto& oldStatus = graph[v].mutate.status;
	if (oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for vert " << v << " to " << newStatus << "\n";

		pruneStack.emplace(std::make_pair(v, oldStatus));
		oldStatus = newStatus;

	}

}

template<typename VProp, typename EProp>
PruneStatus ExtendedSatGraph<VProp, EProp>::get_edge_status(EdgeDescriptor e) const {
	return graph[e].mutate.status;
}

template<typename VProp, typename EProp>
void ExtendedSatGraph<VProp, EProp>::set_edge_status(EdgeDescriptor e, PruneStatus newStatus) {

	auto& oldStatus = graph[e].mutate.status;
	if (oldStatus != newStatus) {

		if (DEBUG) std::cout << "Set status for edge " << e << " to " << newStatus << "\n";

		pruneStack.emplace(std::make_pair(e, oldStatus));
		oldStatus = newStatus;

	}

}

template<typename VProp, typename EProp>
boost::tribool ExtendedSatGraph<VProp, EProp>::get_assignment(VertDescriptor v) const {
	return graph[v].mutate.assignment;
}

template<typename VProp, typename EProp>
void ExtendedSatGraph<VProp, EProp>::set_assignment(VertDescriptor v, boost::tribool newValue) {

	auto& oldValue = graph[v].mutate.assignment;
	if (oldValue != newValue ||
		(!boost::indeterminate(oldValue) && boost::indeterminate(newValue)) ||
		(boost::indeterminate(oldValue) && !boost::indeterminate(newValue))) {

		if (DEBUG) std::cout << "Set assignment for vert " << v << " to " << newValue << "\n";

		pruneStack.emplace(std::make_pair(v, oldValue));
		oldValue = newValue;

	}

}

template<typename VProp, typename EProp>
bool ExtendedSatGraph<VProp, EProp>::is_indeterminate_node(VertDescriptor v) const {

	return (graph[v].base.kind == MutableSatVProp::Node)
		&& (boost::indeterminate(graph[v].mutate.assignment));

}

template<typename VProp, typename EProp>
bool ExtendedSatGraph<VProp, EProp>::is_indeterminate() const {

	auto vPair = boost::vertices(graph);
	return std::any_of(vPair.first, vPair.second, [this](VertDescriptor v) {
		return is_indeterminate_node(v);
	});

}



template<typename VProp, typename EProp>
void ExtendedSatGraph<VProp, EProp>::reverse_to_vert(VertDescriptor v) {

	auto done = false;
	while (!done && !pruneStack.empty()) {

		auto action = pruneStack.top();
		pruneStack.pop();

		using prune_object = PruneAction::PruneObject;
		switch (action.type) {
		case prune_object::Vertex: {

			auto& vertexData =
				boost::get<VertStatusPair>(action.suppData);
			auto vert = vertexData.first;
			auto status = vertexData.second;

			if (DEBUG)
				std::cout << "Prune vert status " << vert << " " << status << "\n";

			graph[vert].mutate.status = status;

			// If vert is prune-to vert, set to done (status is set first, not assignment)
			if (vert == v && status == PruneStatus::Active) done = true;

			break;

		}
		case prune_object::Edge: {

			auto& edgeData =
				boost::get<EdgeStatusPair>(action.suppData);
			auto edge = edgeData.first;
			auto status = edgeData.second;

			if (DEBUG)
				std::cout << "Prune edge status " << edge << " " << status << "\n";

			graph[edge].mutate.status = status;
			break;

		}
		case prune_object::Assignment: {

			auto& incompleteAssignmentData =
				boost::get<IncompleteAssignmentPair>(action.suppData);
			auto vert = incompleteAssignmentData.first;
			auto value = incompleteAssignmentData.second;

			if (DEBUG)
				std::cout << "Prune assignment " << vert << " " << value << "\n";

			graph[vert].mutate.assignment = value;
			break;

		}
		}

	}

}

template<typename VProp, typename EProp>
void ExtendedSatGraph<VProp, EProp>::reset_prune() {

	auto vPair = boost::vertices(graph);
	auto ePair = boost::edges(graph);

	std::for_each(vPair.first, vPair.second, [this](VertDescriptor v) {
		graph[v].mutate.status = PruneStatus::Active;
		graph[v].mutate.assignment = boost::indeterminate;
	});

	std::for_each(ePair.first, ePair.second, [this](EdgeDescriptor e) {
		graph[e].mutate.status = PruneStatus::Active;
	});

	pruneStack = PruneStack();

}





template<typename VProp, typename EProp>
size_t ExtendedSatGraph<VProp, EProp>::num_connected_components() const {
	return numConnectedComponents;
}

template<typename VProp, typename EProp>
const std::vector<VertDescriptor>&
ExtendedSatGraph<VProp, EProp>::connected_component_entry_pts() const {
	return connectedComponentEntryPts;
}



template<typename VProp, typename EProp>
template<typename PruneVisitor>
void ExtendedSatGraph<VProp, EProp>::visit(PruneVisitor& v) {

	visit_sat_graph(v, graph,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}

template<typename VProp, typename EProp>
template<typename PruneVisitor>
void ExtendedSatGraph<VProp, EProp>::visit(PruneVisitor& v) const {

	visit_sat_graph(v, graph,
		connectedComponentEntryPts.begin(), connectedComponentEntryPts.end());

}






template<typename VProp, typename EProp>
SatGraph<VProp, EProp> ExtendedSatGraph<VProp, EProp>::create_extended_from_base(
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
