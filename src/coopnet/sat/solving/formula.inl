

template<typename VProp, typename EProp>
Formula<VProp, EProp>::Formula(const Problem& prob) :
	prob(std::cref(prob)) {

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
template<typename Pred>
void Formula<VProp, EProp>::apply_to_node_vert_map(Pred&& pred) const {
	auto& node_vert_map = prob.get().get_node_vert_map()->left;
	std::for_each(node_vert_map.begin(), node_vert_map.end(), std::forward<Pred>(pred));
}
