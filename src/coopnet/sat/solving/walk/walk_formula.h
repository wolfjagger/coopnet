#pragma once

#include <functional>
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/sat/solving/formula.h"
#include "walk_prop.h"



namespace coopnet {

	class WalkFormula : public Formula<WalkProp> {

	private:

		TranslatedSatGraph<WalkProp> g;

		size_t numClauses;
		size_t numClausesFailed;

	public:

		explicit WalkFormula(const Problem& prob);

		WalkFormula(const WalkFormula& other) = default;
		WalkFormula& operator=(const WalkFormula& other) = default;

		WalkFormula(WalkFormula&& other) = default;
		WalkFormula& operator=(WalkFormula&& other) = default;

		~WalkFormula();



		void flip_node(VertDescriptor vertNode);

		bool is_SAT() const override;

		const Graph& graph() const override;

	protected:

		const SatGraphTranslator& get_sat_graph_translator() const override;

		VertAssignment create_vert_assignment() const override;
		void set_vert_assignment(const VertAssignment& assignment) override;

		Graph& graph() override;

	private:

		void init_clause_satisfaction();

		void sat_clause(VertDescriptor satNode, VertDescriptor vertClause);
		void unsat_clause(VertDescriptor unsatNode, VertDescriptor vertClause);

	};

}
