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
		std::ostream& os, const sat::Node& n) {
		return os << n.id;
	}

	inline std::ostream& operator<<(
		std::ostream& os, const sat::Literal& lit) {
		os << (lit.sgn ? "p" : "n");
		os << lit.n;
		return os;
	}

	inline std::ostream& operator<<(
		std::ostream& os, const sat::Clause& c) {
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
		std::ostream& os, const sat::Assignment& assign) {
		os << "assignment:" << std::endl;
		for (auto iter = assign.data.cbegin();
			iter != assign.data.cend(); ++iter) {
			os << " k" << iter->first << " v" << iter->second ? " T" : " F";
		}
		return os << std::endl;
	}
		
	/*inline std::ostream& operator<<(
		std::ostream& os, const sat::IncompleteAssignment& assign) {
		os << "incomplete_assignment:" << std::endl;
		for (auto iter = assign.data.cbegin();
			iter != assign.data.cend(); ++iter) {
			os << " k" << iter->first << " v" << iter->second;
		}
		return os << std::endl;
	}*/



	inline std::ostream& operator<<(
		std::ostream& os, sat::SolutionStatus status) {

		switch (status) {
		case sat::SolutionStatus::Satisfied:
			os << "Satisfied";
			break;
		case sat::SolutionStatus::Unsatisfiable:
			os << "Unsatisfiable";
			break;
		case sat::SolutionStatus::Undetermined:
			os << "Undetermined";
			break;
		}

		return os;

	}

}}
