#pragma once

#include <functional>
#include "boost/pending/queue.hpp"
#include "coopnet/graph/mutable/reversable_graph.h"
#include "coopnet/sat/solving/formula.h"
#include "dpll_node_choice.h"
#include "dpll_prop.h"



namespace coopnet {

	class Problem;
	class BfsDPLLVisitor;

	class DPLLFormula : public Formula<DPLLProp> {

	private:

		using ReversableGraph = DPLLRevSatGraph;

		ReversableGraph reversableGraph;

		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> greyBuffer;
		// Color for visitation
		SatColorPropMap<DPLLProp> colorPropMap;

		std::shared_ptr<bool> isContradicting;
		std::unique_ptr<BfsDPLLVisitor> pruneVisitor;

	public:

		explicit DPLLFormula(const Problem& prob);

		DPLLFormula(const DPLLFormula& other) = default;
		DPLLFormula& operator=(const DPLLFormula& other) = default;

		DPLLFormula(DPLLFormula&& other) = default;
		DPLLFormula& operator=(DPLLFormula&& other) = default;

		~DPLLFormula();



		void reset() override;



		void set_node(DPLLNodeChoice choice);

		void reverse_prune_to_assignment(VertDescriptor vertNode);



		const ReversableGraph& reversable_graph() const;

		bool is_contradicting() const;
		void set_contradicting();
		void set_uncontradicting();



		IncompleteAssignment create_incomplete_assignment() const;
		void set_incomplete_assignment(const IncompleteAssignment& assignment);

		bool is_SAT() const override;

		const Graph& graph() const override;

	protected:

		const SatGraphTranslator& get_sat_graph_translator() const override;

		VertAssignment create_vert_assignment() const override;
		void set_vert_assignment(const VertAssignment& assignment) override;

		Graph& graph() override;

	};

}
