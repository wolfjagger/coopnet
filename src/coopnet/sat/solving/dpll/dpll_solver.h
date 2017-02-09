#pragma once

#include "boost/optional.hpp"
#include "coopnet/sat/solving/solver.h"
#include "coopnet/graph/graph.h"
#include "dpll_formula.h"



namespace sat {

	class dpll_solver : public complete_solver {

	public:

		// Note: node choice will include sort by largest connection,
		//  watched literals or clauses, most clauses solved, etc.
		enum class node_choice_mode {
			Next, Last, Random
		};

	private:

		std::unique_ptr<dpll_formula> formula;

	public:

		dpll_solver() = default;
	
	protected:
		
		virtual solve_return do_solve(const problem& prob) override;

	private:

		void reduce_formula();
		bool recursive_reduce(node n, bool choice);

		boost::optional<node>
			choose_next_node(node_choice_mode mode) const;

	};

}
