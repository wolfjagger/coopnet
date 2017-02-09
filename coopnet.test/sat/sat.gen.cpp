#include "sat.gen.h"
#include "rc_printing.h"



using namespace rc;

// Nodes
Gen<sat::node> rc::node_gen_with_int_gen(Gen<unsigned int>& int_gen) {
	return gen::construct<sat::node>(int_gen);
}

Gen<sat::node> Arbitrary<sat::node>::arbitrary() {
	return gen::construct<sat::node>(gen::arbitrary<unsigned int>());
}


// Literals
Gen<sat::literal> rc::lit_gen_with_nodes(Gen<sat::node>& node_gen) {
	return gen::construct<sat::literal>(
		node_gen, gen::arbitrary<bool>());
}

Gen<sat::literal> Arbitrary<sat::literal>::arbitrary() {
	return lit_gen_with_nodes(gen::arbitrary<sat::node>());
}


// Clause
Gen<sat::clause> rc::clause_gen_with_nodes(Gen<sat::node>& node_gen) {

	auto lit_list_gen = gen::container<sat::literal::lit_set>(
		lit_gen_with_nodes(node_gen));
	
	auto lam = [](sat::literal::lit_set& set) {
		return sat::clause(set.begin(), set.end());
	};

	return gen::map(lit_list_gen, lam);
	
}

Gen<sat::clause> rc::clause_gen_with_nodes(
	size_t len, Gen<sat::node>& node_gen) {

	auto lit_list_gen
		= gen::container<sat::literal::lit_set>(
			len, lit_gen_with_nodes(node_gen));

	auto lam = [](sat::literal::lit_set& set) {
		return sat::clause(set.begin(), set.end());
	};
	
	return gen::map(lit_list_gen, lam);

}

Gen<sat::clause> Arbitrary<sat::clause>::arbitrary() {
	return clause_gen_with_nodes(gen::arbitrary<sat::node>());
}


// Tribool
Gen<boost::logic::tribool> Arbitrary<boost::logic::tribool>::arbitrary() {

	return gen::map(gen::inRange(0, 3), [](int choice) {
		if (choice == 0) {
			return boost::logic::tribool(true);
		} else if (choice == 1) {
			return boost::logic::tribool(boost::indeterminate);
		} else {
			return boost::logic::tribool(false);
		}
	});

}


// Assignment
Gen<sat::assignment> Arbitrary<sat::assignment>::arbitrary() {
	return gen::build<sat::assignment>(
		gen::set(&sat::assignment::data));
}

Gen<sat::incomplete_assignment>
Arbitrary<sat::incomplete_assignment>::arbitrary() {

	return gen::build<sat::incomplete_assignment>(
		gen::set(&sat::incomplete_assignment::data,
			gen::container<sat::incomplete_assignment::map>(
				gen::arbitrary<sat::node>(),
				gen::arbitrary<boost::logic::tribool>())));
	
}
