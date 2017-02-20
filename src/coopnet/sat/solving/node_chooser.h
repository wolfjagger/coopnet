#pragma once

#include "boost/optional.hpp"
#include "coopnet/sat/problem/assignment.h"
#include "node_choice.h"



namespace sat {

	class Formula;

	// Note: Will include sort by largest connection,
	//  watched literals or clauses, most clauses solved, etc.
	class NodeChooser {

	protected:

		using AssignmentMap = IncompleteAssignment::Map;
		using VertChoice = std::pair<VertDescriptor, bool>;

	public:

		boost::optional<NodeChoice> choose(const Formula& form);

	protected:

		virtual VertChoice do_choose(
			const Formula& form, const AssignmentMap& assign_map) = 0;

	};





	class NextNodeChooser : public NodeChooser {

	protected:

		VertChoice do_choose(
			const Formula& form, const AssignmentMap& assign_map) override;

	};


	class LastNodeChooser : public NodeChooser {

	protected:

		VertChoice do_choose(
			const Formula& form, const AssignmentMap& assign_map) override;

	};


	class RandNodeChooser : public NodeChooser {

	protected:

		VertChoice do_choose(
			const Formula& form, const AssignmentMap& assign_map) override;

	};

}
