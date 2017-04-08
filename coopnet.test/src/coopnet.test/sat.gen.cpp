#include "sat.gen.h"
#include "rc_printing.h"



using namespace rc;

// Nodes
Gen<coopnet::Node> rc::node_gen_with_int_gen(Gen<unsigned int>& int_gen) {
	return gen::construct<coopnet::Node>(int_gen);
}

Gen<coopnet::Node> Arbitrary<coopnet::Node>::arbitrary() {
	return gen::construct<coopnet::Node>(gen::arbitrary<unsigned int>());
}


// Literals
Gen<coopnet::Literal> rc::lit_gen_with_nodes(Gen<coopnet::Node>& node_gen) {
	return gen::construct<coopnet::Literal>(
		node_gen, gen::arbitrary<bool>());
}

Gen<coopnet::Literal> Arbitrary<coopnet::Literal>::arbitrary() {
	return lit_gen_with_nodes(gen::arbitrary<coopnet::Node>());
}


// Clause
Gen<coopnet::Clause> rc::clause_gen_with_nodes(Gen<coopnet::Node>& node_gen) {

	auto lit_list_gen = gen::container<coopnet::Literal::LitSet>(
		lit_gen_with_nodes(node_gen));
	
	auto lam = [](coopnet::Literal::LitSet& set) {
		return coopnet::Clause(set.begin(), set.end());
	};

	return gen::map(lit_list_gen, lam);
	
}

Gen<coopnet::Clause> rc::clause_gen_with_nodes(
	size_t len, Gen<coopnet::Node>& node_gen) {

	auto lit_list_gen
		= gen::container<coopnet::Literal::LitSet>(
			len, lit_gen_with_nodes(node_gen));

	auto lam = [](coopnet::Literal::LitSet& set) {
		return coopnet::Clause(set.begin(), set.end());
	};
	
	return gen::map(lit_list_gen, lam);

}

Gen<coopnet::Clause> Arbitrary<coopnet::Clause>::arbitrary() {
	return clause_gen_with_nodes(gen::arbitrary<coopnet::Node>());
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
Gen<coopnet::Assignment> Arbitrary<coopnet::Assignment>::arbitrary() {
	return gen::build<coopnet::Assignment>(
		gen::set(&coopnet::Assignment::data));
}
