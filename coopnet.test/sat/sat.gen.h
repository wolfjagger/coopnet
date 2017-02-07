#pragma once

#include "rapidcheck.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/problem/assignment.h"



namespace rc {

	// Nodes
	Gen<sat::node> node_gen_with_int_gen(
		Gen<unsigned int>& int_gen);

	template<>
	struct Arbitrary<sat::node> {
		static Gen<sat::node> arbitrary();
	};

	// Literals
	Gen<sat::literal> lit_gen_with_nodes(
		Gen<sat::node>& node_gen);

	template<>
	struct Arbitrary<sat::literal> {
		static Gen<sat::literal> arbitrary();
	};

	// Clause
	Gen<sat::clause> clause_gen_with_nodes(
		Gen<sat::node>& node_gen);

	Gen<sat::clause> clause_gen_with_nodes(
		size_t len, Gen<sat::node>& node_gen);

	template<>
	struct Arbitrary<sat::clause> {
		static Gen<sat::clause> arbitrary();
	};

	// Tribool
	template<>
	struct Arbitrary<boost::logic::tribool> {
		static Gen<boost::logic::tribool> arbitrary();
	};

	// Assignment
	template<>
	struct Arbitrary<sat::assignment> {
		static Gen<sat::assignment> arbitrary();
	};

	template<>
	struct Arbitrary<sat::incomplete_assignment> {
		static Gen<sat::incomplete_assignment> arbitrary();
	};

}
