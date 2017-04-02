#pragma once

#include <functional>
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/sat/solving/formula.h"
#include "walk_prop.h"



namespace coopnet {

	class Problem;
	struct BfsWalkVisitor;

	class WalkFormula : public Formula<WalkVProp, WalkEProp> {

	private:

		Graph& g;

		size_t numClauses;
		size_t numClausesFailed;

	public:

		explicit WalkFormula(const Problem& prob);

		WalkFormula(const WalkFormula& other) = default;
		WalkFormula& operator=(const WalkFormula& other) = default;

		WalkFormula(WalkFormula&& other) = default;
		WalkFormula& operator=(WalkFormula&& other) = default;

		~WalkFormula();



		void flip_node(Node node);

		bool is_SAT() const override;
		Assignment create_assignment() const override;
		void set_assignment(const Assignment& assignment) override;

		const Graph& graph() const override;

	protected:

		Graph& graph() override;

	private:

		void init_clause_satisfaction();

		void satisfy_clause(VertDescriptor vertClause);
		void check_unsatisfied_clause(VertDescriptor vertClause);

	};

}
