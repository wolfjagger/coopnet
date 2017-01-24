#include "dpll_formula.h"
#include "sat/problem.h"

using namespace sat;



dpll_formula::dpll_formula(const problem& prob) :
	partial_graph(prob.get_graph()),
	partial_assign(prob),
	prune_action_stack() {

}



void dpll_formula::set_node(vertex_descriptor node, bool value) {

	// partial_assign set @ node_to_set=value
	// partial_graph remove node_to_set and reduce (unit clauses
	//  & pure literals), supplying the stack to append

}

void dpll_formula::reverse_prune_to_node(vertex_descriptor node) {

	// while !done
	//  pop action off stack
	//  parse and interpret action
	//  apply reverse action on graph
	//  done = (action.type == Node && action.data.descriptor == node)

}
