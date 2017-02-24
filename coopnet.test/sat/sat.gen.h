#pragma once

#include "rapidcheck.h"
#include "boost/logic/tribool.hpp"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/problem/assignment.h"



namespace rc {

	// Nodes
	Gen<coopnet::Node> node_gen_with_int_gen(
		Gen<unsigned int>& int_gen);

	template<>
	struct Arbitrary<coopnet::Node> {
		static Gen<coopnet::Node> arbitrary();
	};

	// Literals
	Gen<coopnet::Literal> lit_gen_with_nodes(
		Gen<coopnet::Node>& node_gen);

	template<>
	struct Arbitrary<coopnet::Literal> {
		static Gen<coopnet::Literal> arbitrary();
	};

	// Clause
	Gen<coopnet::Clause> clause_gen_with_nodes(
		Gen<coopnet::Node>& node_gen);

	Gen<coopnet::Clause> clause_gen_with_nodes(
		size_t len, Gen<coopnet::Node>& node_gen);

	template<>
	struct Arbitrary<coopnet::Clause> {
		static Gen<coopnet::Clause> arbitrary();
	};

	// Tribool
	template<>
	struct Arbitrary<boost::logic::tribool> {
		static Gen<boost::logic::tribool> arbitrary();
	};

	// Assignment
	template<>
	struct Arbitrary<coopnet::Assignment> {
		static Gen<coopnet::Assignment> arbitrary();
	};

/*	template<>
	struct Arbitrary<sat::IncompleteAssignment> {
		static Gen<sat::IncompleteAssignment> arbitrary();
	};*/

}
