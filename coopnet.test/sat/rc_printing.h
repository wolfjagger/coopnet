#pragma once

#include <ostream>
#include "rapidcheck.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "coopnet/sat/assignment/assignment.h"



namespace rc { namespace detail {
		
	inline std::ostream& operator<<(
		std::ostream& os, const sat::node& n) {
		return os << n.id;
	}

	inline std::ostream& operator<<(
		std::ostream& os, const sat::clause& c) {
		for (auto i = 0; i < c.size(); ++i) {
			os << (c.sgns().at(i) ? "+" : "-");
			os << c.nodes().at(i) << " ";
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
			os << " k" << iter->first << " v" << iter->second;
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

}}
