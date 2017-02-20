#pragma once

#include "rapidcheck.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/problem/assignment.h"



namespace rc {

	// Nodes
	Gen<sat::Node> node_gen_with_int_gen(
		Gen<unsigned int>& int_gen);

	template<>
	struct Arbitrary<sat::Node> {
		static Gen<sat::Node> arbitrary();
	};

	// Literals
	Gen<sat::Literal> lit_gen_with_nodes(
		Gen<sat::Node>& node_gen);

	template<>
	struct Arbitrary<sat::Literal> {
		static Gen<sat::Literal> arbitrary();
	};

	// Clause
	Gen<sat::Clause> clause_gen_with_nodes(
		Gen<sat::Node>& node_gen);

	Gen<sat::Clause> clause_gen_with_nodes(
		size_t len, Gen<sat::Node>& node_gen);

	template<>
	struct Arbitrary<sat::Clause> {
		static Gen<sat::Clause> arbitrary();
	};

	// Tribool
	template<>
	struct Arbitrary<boost::logic::tribool> {
		static Gen<boost::logic::tribool> arbitrary();
	};

	// Assignment
	template<>
	struct Arbitrary<sat::Assignment> {
		static Gen<sat::Assignment> arbitrary();
	};

	template<>
	struct Arbitrary<sat::IncompleteAssignment> {
		static Gen<sat::IncompleteAssignment> arbitrary();
	};

}
