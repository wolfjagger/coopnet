#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/solving/solver.h"



namespace rc { namespace detail {
		
	inline std::ostream& operator<<(
		std::ostream& os, const sat::node& n) {
		return os << n.id;
	}

	inline std::ostream& operator<<(
		std::ostream& os, const sat::literal& lit) {
		os << (lit.sgn ? "p" : "n");
		os << lit.n;
		return os;
	}

	inline std::ostream& operator<<(
		std::ostream& os, const sat::clause& c) {
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


	inline std::ostream& operator<<(
		std::ostream& os, const sat::assignment& assign) {
		os << "assignment:" << std::endl;
		for (auto iter = assign.data.cbegin();
			iter != assign.data.cend(); ++iter) {
			os << " k" << iter->first << " v" << iter->second ? " T" : " F";
		}
		return os << std::endl;
	}
		
	inline std::ostream& operator<<(
		std::ostream& os, const sat::incomplete_assignment& assign) {
		os << "incomplete_assignment:" << std::endl;
		for (auto iter = assign.data.cbegin();
			iter != assign.data.cend(); ++iter) {
			os << " k" << iter->first << " v" << iter->second;
		}
		return os << std::endl;
	}



	inline std::ostream& operator<<(
		std::ostream& os, sat::solution_status status) {

		switch (status) {
		case sat::solution_status::Satisfied:
			os << "Satisfied";
			break;
		case sat::solution_status::Unsatisfiable:
			os << "Unsatisfiable";
			break;
		case sat::solution_status::Undetermined:
			os << "Undetermined";
			break;
		}

		return os;

	}

}}
