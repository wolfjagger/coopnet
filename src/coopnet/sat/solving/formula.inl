

template<typename VProp, typename EProp>
Formula<VProp, EProp>::Formula(const Problem& prob) :
	prob(std::cref(prob)) {

}



template<typename VProp, typename EProp>
Assignment Formula<VProp, EProp>::create_assignment() const {

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

template<typename VProp, typename EProp>
void Formula<VProp, EProp>::set_assignment(const Assignment& assignment) {

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
