#include "dpll_node_chooser.h"
#include "dpll_formula.h"
#include "visitor/dpll_choose_node_visitor.h"



using namespace coopnet;



auto DPLLNodeChooser::do_choose(const Formula& form)
	-> VertChoice {

	auto& dpll_form = static_cast<const DPLLFormula&>(form);
	auto visitor = DPLLChooseNodeVisitor(form.prune_graph().prune_info());

	form.visit_active_graph(visitor);

	return visitor.retreive_choice();

}
