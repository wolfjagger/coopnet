

template<typename VProp, typename EProp>
Formula<VProp, EProp>::Formula(const Problem& prob) :
	prob(std::cref(prob)),
	reversableGraph(prob.get_graph()) {

	colorPropMap
		= boost::get(&VProp::color, reversableGraph.get_graph());

}



template<typename VProp, typename EProp>
void Formula<VProp, EProp>::reverse_prune_to_assignment(Node n) {

	auto vert = node_vert_map().left.at(n);
	reversableGraph.reverse_to_vert(vert);

}



template<typename VProp, typename EProp>
const NodeVertMap& Formula<VProp, EProp>::node_vert_map() const {
	return *node_vert_map_ptr();
}
template<typename VProp, typename EProp>
std::shared_ptr<const NodeVertMap> Formula<VProp, EProp>::node_vert_map_ptr() const {
	return prob.get().get_node_vert_map();
}



template<typename VProp, typename EProp>
auto Formula<VProp, EProp>::reversable_graph() const
    -> const ReversableGraph& {
	return reversableGraph;
}
template<typename VProp, typename EProp>
auto Formula<VProp, EProp>::graph() const 
    -> const Graph& {
	return reversableGraph.get_graph();
}



template<typename VProp, typename EProp>
bool Formula<VProp, EProp>::is_SAT() const {
	return !reversableGraph.is_indeterminate();
}



namespace {
	template<typename Pred>
	void apply_to_node_vert_map(const Problem& prob, Pred&& pred) {
		auto& node_vert_map = prob.get_node_vert_map()->left;
		std::for_each(node_vert_map.begin(), node_vert_map.end(), std::forward<Pred>(pred));
	}
}

template<typename VProp, typename EProp>
IncompleteAssignment Formula<VProp, EProp>::create_incomplete_assignment() const {

	auto assignment = IncompleteAssignment();
	auto copy_pred = [this, &assignment](auto pair) {
		assignment.emplace(pair.second, reversableGraph.get_assignment(pair.second));
	};
	apply_to_node_vert_map(prob.get(), copy_pred);

	return assignment;

}

template<typename VProp, typename EProp>
void Formula<VProp, EProp>::set_incomplete_assignment(const IncompleteAssignment& assignment) {

	auto copy_pred = [this, &assignment](auto pair) {
		reversableGraph.set_assignment(pair.second, assignment.at(pair.second));
	};
	apply_to_node_vert_map(prob.get(), copy_pred);

}

template<typename VProp, typename EProp>
Assignment Formula<VProp, EProp>::create_assignment() const {

	if (reversableGraph.is_indeterminate()) {
		throw std::exception("Incomplete assignment cannot be transformed.");
	}

	Assignment assignment;
	auto copy_pred = [this, &assignment](auto pair) {
		assignment.data.emplace(pair.first, bool(reversableGraph.get_assignment(pair.second)));
	};
	apply_to_node_vert_map(prob.get(), copy_pred);

	return assignment;

}

template<typename VProp, typename EProp>
void Formula<VProp, EProp>::set_assignment(const Assignment& assignment) {

	auto copy_pred = [this, &assignment](auto pair) {
		reversableGraph.set_assignment(pair.second, boost::tribool(assignment.data.at(pair.first)));
	};
	apply_to_node_vert_map(prob.get(), copy_pred);

}
