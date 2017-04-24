

template<typename SatProp>
Formula<SatProp>::Formula(const Problem& prob) :
	prob(std::cref(prob)) {

}



template<typename SatProp>
Assignment Formula<SatProp>::create_assignment() const {

	auto& translator = get_sat_graph_translator();
	auto assign = Assignment();

	auto vertAssign = create_vert_assignment();

	for (auto iter = vertAssign.begin(); iter != vertAssign.end(); ++iter) {

		auto vert = iter->first;
		auto sgn = iter->second;

		assign.data.emplace(translator.vert_to_node(vert), sgn);

	}

	return assign;

}

template<typename SatProp>
void Formula<SatProp>::set_assignment(const Assignment& assignment) {

	auto& translator = get_sat_graph_translator();
	auto assign = VertAssignment();

	for (auto iter = assignment.data.begin(); iter != assignment.data.end(); ++iter) {

		auto node = iter->first;
		auto sgn = iter->second;

		auto vert = translator.node_to_vert(node);
		assign.emplace(vert, sgn);

	}

	set_vert_assignment(assign);

}



template<typename SatProp>
auto Formula<SatProp>::node_begin() const {
	return get_sat_graph_translator().node_begin();
}
template<typename SatProp>
auto Formula<SatProp>::node_end() const {
	return get_sat_graph_translator().node_end();
}
