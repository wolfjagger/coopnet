#pragma once

#include "boost/optional.hpp"
#include "coopnet/sat/problem/assignment.h"
#include "node_choice.h"



namespace sat {

	class formula;

	// Note: Will include sort by largest connection,
	//  watched literals or clauses, most clauses solved, etc.
	class node_chooser {

	protected:

		using assignment_map = incomplete_assignment::map;
		using vert_choice = std::pair<vertex_descriptor, bool>;

	public:

		boost::optional<node_choice> choose(const formula& form);

	protected:

		virtual vert_choice do_choose(
			const formula& form, const assignment_map& assign_map) = 0;

	};





	class next_node_chooser : public node_chooser {

	protected:

		vert_choice do_choose(
			const formula& form, const assignment_map& assign_map) override;

	};


	class last_node_chooser : public node_chooser {

	protected:

		vert_choice do_choose(
			const formula& form, const assignment_map& assign_map) override;

	};


	class rand_node_chooser : public node_chooser {

	protected:

		vert_choice do_choose(
			const formula& form, const assignment_map& assign_map) override;

	};

}
