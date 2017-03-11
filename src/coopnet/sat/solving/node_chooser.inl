

template<class ConcreteFormula>
boost::optional<NodeChoice> NodeChooser<ConcreteFormula>::choose(
	const ConcreteFormula& form) {

	auto& extendedGraph = form.extended_graph();

	// If none, return no node
	if (!extendedGraph.is_indeterminate()) {
		return boost::optional<NodeChoice>();
	} else {

		auto choice = do_choose(form);

		auto n = form.node_vert_map().right.at(choice.first);

		return NodeChoice{ n, choice.second };

	}

}





template<class ConcreteFormula>
auto NextNodeChooser<ConcreteFormula>::do_choose(const ConcreteFormula& form)
-> VertChoice {

	auto& extendedGraph = form.extended_graph();

	auto vPair = boost::vertices(extendedGraph.get_graph());

	auto iter = std::find_if(vPair.first, vPair.second,
		[&extendedGraph](VertDescriptor v) {

		return extendedGraph.is_indeterminate_node(v);

	});

	return std::make_pair(*iter, true);

}



namespace detail {
	static auto rand_node_engine = std::mt19937_64(std::random_device()());
}

template<class ConcreteFormula>
auto RandNodeChooser<ConcreteFormula>::do_choose(const ConcreteFormula& form)
	-> VertChoice {

	auto& extendedGraph = form.extended_graph();

	auto vPair = boost::vertices(extendedGraph.get_graph());

	auto iter = alphali::random_find_if(vPair.first, vPair.second,
		[&extendedGraph](VertDescriptor v) {

		return extendedGraph.is_indeterminate_node(v);

	}, detail::rand_node_engine);

	return std::make_pair(*iter, true);

}



template<class ConcreteFormula>
auto MaxClauseNodeChooser<ConcreteFormula>::do_choose(const ConcreteFormula& form)
    -> VertChoice {

	auto visitor = ChooseMaxClausesVisitor();

	form.visit_active_graph(visitor);

	return visitor.retreive_choice();

}
