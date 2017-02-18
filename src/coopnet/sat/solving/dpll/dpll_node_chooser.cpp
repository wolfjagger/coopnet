#include "dpll_node_chooser.h"
#include "dpll_formula.h"
#include "visitor/dpll_choose_node_visitor.h"



using namespace sat;



vertex_descriptor dpll_node_chooser::do_choose(
	const formula& form, const assignment_map& assign_map) {

	auto& dpll_form = static_cast<const dpll_formula&>(form);
	dpll_choose_node_visitor visitor(dpll_form.get_prop_maps());

	form.visit_problem(visitor);

	return visitor.get_chosen_node();

}
