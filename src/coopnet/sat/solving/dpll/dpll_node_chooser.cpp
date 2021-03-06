#include "dpll_node_chooser.h"
#include "alphali/containers/random_iterator.h"
#include "coopnet/graph/mutable/reversable_graph.h"
#include "coopnet/sat/visitor/prune/choose_max_clauses_visitor.h"
#include "dpll_formula.h"

using namespace coopnet;



DPLLNodeChooser::DPLLNodeChooser() :
	form(nullptr) {
	
}

DPLLNodeChooser::DPLLNodeChooser(const DPLLFormula& initForm) :
	form(&initForm) {

}



DPLLNodeChooser::~DPLLNodeChooser() { }



std::unique_ptr<DPLLNodeChooser> DPLLNodeChooser::create(DPLLNodeChoiceMode mode) {

	switch (mode) {
	case DPLLNodeChoiceMode::Next:
		return std::make_unique<NextNodeChooser>();
	case DPLLNodeChoiceMode::Random:
		return std::make_unique<RandNodeChooser>();
	case DPLLNodeChoiceMode::MostSameClauses:
		return std::make_unique<MaxSameClauseNodeChooser>();
	case DPLLNodeChoiceMode::MostTotClauses:
		return std::make_unique<MaxTotClauseNodeChooser>();
	default:
		throw std::exception("Unknown DPLL node choice mode.");
	}

}



void DPLLNodeChooser::set_formula(const DPLLFormula& newFormula) {
	form = &newFormula;
	do_set_formula();
}



boost::optional<DPLLNodeChoice> DPLLNodeChooser::choose() {

	if (!form) throw std::exception("Formula needs to be set before choosing node.");

	auto& reversableGraph = form->reversable_graph();

	// If none, return no node
	if (!reversableGraph.is_indeterminate()) {
		return boost::optional<DPLLNodeChoice>();
	} else {

		return do_choose();

	}

}





NextNodeChooser::NextNodeChooser(const DPLLFormula& initForm) :
	DPLLNodeChooser(initForm) {

}

DPLLNodeChoice NextNodeChooser::do_choose() {

	auto& reversableGraph = form->reversable_graph();

	auto vPair = boost::vertices(reversableGraph.get_graph());

	auto iter = std::find_if(vPair.first, vPair.second,
		[&reversableGraph](VertDescriptor v) {

		return reversableGraph.is_indeterminate_node(v);

	});

	return std::make_pair(*iter, true);

}



RandNodeChooser::RandNodeChooser(const DPLLFormula& initForm) :
	DPLLNodeChooser(initForm) {

}

namespace detail {
	static auto rand_node_engine = std::mt19937_64(std::random_device()());
}

DPLLNodeChoice RandNodeChooser::do_choose() {

	auto& reversableGraph = form->reversable_graph();

	auto vPair = boost::vertices(reversableGraph.get_graph());

	auto iter = alphali::random_find_if(vPair.first, vPair.second,
		[&reversableGraph](VertDescriptor v) {

		return reversableGraph.is_indeterminate_node(v);

	}, detail::rand_node_engine);

	return std::make_pair(*iter, true);

}



MaxSameClauseNodeChooser::MaxSameClauseNodeChooser(const DPLLFormula& initForm) :
	DPLLNodeChooser(initForm) {

}

DPLLNodeChoice MaxSameClauseNodeChooser::do_choose() {

	auto visitor = ChooseMaxSameClausesVisitor();

	form->reversable_graph().visit(visitor);

	return visitor.retreive_choice();

}



MaxTotClauseNodeChooser::MaxTotClauseNodeChooser(const DPLLFormula& initForm) :
	DPLLNodeChooser(initForm) {

}

DPLLNodeChoice MaxTotClauseNodeChooser::do_choose() {

	auto visitor = ChooseMaxTotClausesVisitor();

	form->reversable_graph().visit(visitor);

	return visitor.retreive_choice();

}
