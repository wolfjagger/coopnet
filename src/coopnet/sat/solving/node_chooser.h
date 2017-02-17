#pragma once

#include "boost/optional.hpp"
#include "coopnet/sat/problem/assignment.h"



namespace sat {

	class formula;

	// Note: Will include sort by largest connection,
	//  watched literals or clauses, most clauses solved, etc.
	class node_chooser {

	protected:

		using assignment_map = incomplete_assignment::map;

	public:

		boost::optional<node> choose(const formula& form);

	protected:

		virtual vertex_descriptor do_choose(const assignment_map& assign_map) = 0;

	};





	class next_node_chooser : public node_chooser {

	protected:

		vertex_descriptor do_choose(const assignment_map& assign_map) override;

	};


	class last_node_chooser : public node_chooser {

	protected:

		vertex_descriptor do_choose(const assignment_map& assign_map) override;

	};


	class rand_node_chooser : public node_chooser {

	protected:

		vertex_descriptor do_choose(const assignment_map& assign_map) override;

	};

}
