

template<typename VProp, typename EProp>
Formula<VProp, EProp>::Formula(const Problem& prob) :
	prob(std::cref(prob)),
	extendedGraph(prob.get_graph()) {

}



template<typename VProp, typename EProp>
void Formula<VProp, EProp>::reverse_prune_to_assignment(Node n) {

	auto vert = node_vert_map().left.at(n);
	extendedGraph.reverse_to_vert(vert);

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
auto Formula<VProp, EProp>::extended_graph() const
    -> const ExtendedGraph& {
	return extendedGraph;
}
template<typename VProp, typename EProp>
auto Formula<VProp, EProp>::graph() const 
    -> const Graph& {
	return extendedGraph.get_graph();
}



template<typename VProp, typename EProp>
bool Formula<VProp, EProp>::is_SAT() const {
	return !extendedGraph.is_indeterminate();
}

template<typename VProp, typename EProp>
IncompleteAssignment Formula<VProp, EProp>::create_incomplete_assignment() const {

	auto assign = IncompleteAssignment();
	auto copy_pred = [this, &assign](auto pair) {
		assign.emplace(pair.second, extendedGraph.get_assignment(pair.second));
	};

	auto& node_vert_map = prob.get().get_node_vert_map()->left;
	std::for_each(node_vert_map.begin(), node_vert_map.end(), copy_pred);

	return assign;

}

template<typename VProp, typename EProp>
Assignment Formula<VProp, EProp>::create_assignment() const {

	if (extendedGraph.is_indeterminate()) {
		throw std::exception("Incomplete assignment cannot be transformed.");
	}

	Assignment assign;
	auto copy_pred = [this, &assign](auto pair) {
		assign.data.emplace(pair.first, bool(extendedGraph.get_assignment(pair.second)));
	};

	auto& node_vert_map = prob.get().get_node_vert_map()->left;
	std::for_each(node_vert_map.begin(), node_vert_map.end(), copy_pred);

	return assign;

}
