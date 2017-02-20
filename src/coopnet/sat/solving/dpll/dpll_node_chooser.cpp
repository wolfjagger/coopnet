#include "dpll_node_chooser.h"
#include "dpll_formula.h"
#include "visitor/dpll_choose_node_visitor.h"



using namespace sat;



auto DPLLNodeChooser::do_choose(
	const Formula& form, const AssignmentMap& assign_map)
	-> VertChoice {

	auto& dpll_form = static_cast<const DPLLFormula&>(form);
	DPLLChooseNodeVisitor visitor(dpll_form.get_prop_maps());

	form.visit_problem(visitor);

	return visitor.retreive_choice();

}
