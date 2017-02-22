#include "sat.gen.h"
#include "rc_printing.h"



using namespace rc;

// Nodes
Gen<sat::Node> rc::node_gen_with_int_gen(Gen<unsigned int>& int_gen) {
	return gen::construct<sat::Node>(int_gen);
}

Gen<sat::Node> Arbitrary<sat::Node>::arbitrary() {
	return gen::construct<sat::Node>(gen::arbitrary<unsigned int>());
}


// Literals
Gen<sat::Literal> rc::lit_gen_with_nodes(Gen<sat::Node>& node_gen) {
	return gen::construct<sat::Literal>(
		node_gen, gen::arbitrary<bool>());
}

Gen<sat::Literal> Arbitrary<sat::Literal>::arbitrary() {
	return lit_gen_with_nodes(gen::arbitrary<sat::Node>());
}


// Clause
Gen<sat::Clause> rc::clause_gen_with_nodes(Gen<sat::Node>& node_gen) {

	auto lit_list_gen = gen::container<sat::Literal::LitSet>(
		lit_gen_with_nodes(node_gen));
	
	auto lam = [](sat::Literal::LitSet& set) {
		return sat::Clause(set.begin(), set.end());
	};

	return gen::map(lit_list_gen, lam);
	
}

Gen<sat::Clause> rc::clause_gen_with_nodes(
	size_t len, Gen<sat::Node>& node_gen) {

	auto lit_list_gen
		= gen::container<sat::Literal::LitSet>(
			len, lit_gen_with_nodes(node_gen));

	auto lam = [](sat::Literal::LitSet& set) {
		return sat::Clause(set.begin(), set.end());
	};
	
	return gen::map(lit_list_gen, lam);

}

Gen<sat::Clause> Arbitrary<sat::Clause>::arbitrary() {
	return clause_gen_with_nodes(gen::arbitrary<sat::Node>());
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
Gen<sat::Assignment> Arbitrary<sat::Assignment>::arbitrary() {
	return gen::build<sat::Assignment>(
		gen::set(&sat::Assignment::data));
}

/*Gen<sat::IncompleteAssignment>
Arbitrary<sat::IncompleteAssignment>::arbitrary() {

	return gen::build<sat::IncompleteAssignment>(
		gen::set(&sat::IncompleteAssignment::data,
			gen::container<sat::IncompleteAssignment::Map>(
				gen::arbitrary<sat::VertDescriptor>(),
				gen::arbitrary<boost::logic::tribool>())));
	
}*/
