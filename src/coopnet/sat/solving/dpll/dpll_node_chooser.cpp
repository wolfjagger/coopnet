#include "dpll_node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "coopnet/graph/mutable/reversable_graph.h"
#include "coopnet/sat/visitor/choose_max_clauses_visitor.h"
#include "dpll_formula.h"

using namespace coopnet;



boost::optional<DPLLNodeChoice> DPLLNodeChooser::choose(
	const DPLLFormula& form) {

	auto& reversableGraph = form.reversable_graph();

	// If none, return no node
	if (!reversableGraph.is_indeterminate()) {
		return boost::optional<DPLLNodeChoice>();
	} else {

		auto choice = do_choose(form);

		auto n = form.node_vert_map().right.at(choice.first);

		return DPLLNodeChoice{ n, choice.second };

	}

}





auto NextNodeChooser::do_choose(const DPLLFormula& form)
-> VertChoice {

	auto& reversableGraph = form.reversable_graph();

	auto vPair = boost::vertices(reversableGraph.get_graph());

	auto iter = std::find_if(vPair.first, vPair.second,
		[&reversableGraph](VertDescriptor v) {

		return reversableGraph.is_indeterminate_node(v);

	});

	return std::make_pair(*iter, true);

}



namespace detail {
	static auto rand_node_engine = std::mt19937_64(std::random_device()());
}

auto RandNodeChooser::do_choose(const DPLLFormula& form)
	-> VertChoice {

	auto& reversableGraph = form.reversable_graph();

	auto vPair = boost::vertices(reversableGraph.get_graph());

	auto iter = alphali::random_find_if(vPair.first, vPair.second,
		[&reversableGraph](VertDescriptor v) {

		return reversableGraph.is_indeterminate_node(v);

	}, detail::rand_node_engine);

	return std::make_pair(*iter, true);

}



auto MaxSameClauseNodeChooser::do_choose(const DPLLFormula& form)
    -> VertChoice {

	auto visitor = ChooseMaxSameClausesVisitor();

	form.reversable_graph().visit(visitor);

	return visitor.retreive_choice();

}



auto MaxTotClauseNodeChooser::do_choose(const DPLLFormula& form)
-> VertChoice {

	auto visitor = ChooseMaxTotClausesVisitor();

	form.reversable_graph().visit(visitor);

	return visitor.retreive_choice();

}
