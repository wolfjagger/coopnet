#pragma once

#include "boost/logic/tribool.hpp"
#include "boost/optional.hpp"
#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "node_choice.h"



namespace sat {

	class Formula;

	// Note: Will include sort by largest connection,
	//  watched literals or clauses, most clauses solved, etc.
	class NodeChooser {

	protected:

		using VertChoice = std::pair<VertDescriptor, bool>;

	public:

		boost::optional<NodeChoice> choose(const Formula& form);

	protected:

		virtual VertChoice do_choose(const Formula& form) = 0;

		static bool is_ind_pair(std::pair<VertDescriptor, boost::tribool> pair);

	};





	class NextNodeChooser : public NodeChooser {

	protected:

		VertChoice do_choose(const Formula& form) override;

	};


	class LastNodeChooser : public NodeChooser {

	protected:

		VertChoice do_choose(const Formula& form) override;

	};


	class RandNodeChooser : public NodeChooser {

	protected:

		VertChoice do_choose(const Formula& form) override;

	};

}
