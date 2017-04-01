#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "boost/logic/tribool.hpp"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/solving/solver.h"



namespace rc { namespace detail {

	using coopnet::operator<<;
	
	inline std::ostream& operator<<(
		std::ostream& os, const coopnet::Node& n) {
		return os << n.id;
	}

	inline std::ostream& operator<<(
		std::ostream& os, const coopnet::Literal& lit) {
		os << (lit.sgn ? "p" : "n");
		os << lit.n;
		return os;
	}

	inline std::ostream& operator<<(
		std::ostream& os, const coopnet::Clause& c) {
		for (auto lit : c.literals()) {
			os << (lit.second ? "p" : "n");
			os << lit.first << " ";
		}
		return os;
	}


	inline std::ostream& operator<<(
		std::ostream& os, const boost::logic::tribool& b) {
		if (b) {
			os << "T";
		} else if (!b) {
			os << "F";
		} else {
			os << "I";
		}
		return os;
	}

}}
