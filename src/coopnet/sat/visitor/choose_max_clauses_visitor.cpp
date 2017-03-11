#include "choose_max_clauses_visitor.h"

using namespace coopnet;



ChooseMaxClausesVisitor::ChooseMaxClausesVisitor() {

	reset();

}



void ChooseMaxClausesVisitor::reset() {

	max_num_clauses_sat = -1;
	chosen_node = std::make_shared<VertDescriptor>();
	sgn_sat = std::make_shared<bool>(false);

}



std::pair<VertDescriptor, bool> ChooseMaxClausesVisitor::retreive_choice() const {
	return std::make_pair(*chosen_node, *sgn_sat);
}
