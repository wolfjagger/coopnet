#pragma once

#include <functional>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/solving/formula.h"
#include "visitor/dpll_visitor.h"



namespace sat {

	class Problem;

	class DPLLFormula : public Formula {

	private:

		bool contradicting;
		// Could also put this in dpll_solver or in local function scope
		PruneStack prune_action_stack;
		// Queue for remaining grey nodes to color black
		boost::queue<VertDescriptor> grey_buffer;

		// For property maps associated with vert/edge statuses
		DPLLVertStatusMap vert_status_map;
		DPLLEdgeStatusMap edge_status_map;
		// Vert color map primarily controlled dynamically by visitor
		DPLLColorMap color_map;

		DPLLPropMaps prop_maps;

		std::unique_ptr<DPLLVisitor> prune_visitor;

	public:

		explicit DPLLFormula(const Problem& prob);

		DPLLFormula(const DPLLFormula& other) = default;
		DPLLFormula& operator=(const DPLLFormula& other) = default;

		DPLLFormula(DPLLFormula&& other) = default;
		DPLLFormula& operator=(DPLLFormula&& other) = default;

		~DPLLFormula() = default;



		void set_node(NodeChoice choice);

		void reverse_prune_to_assignment(Node n);

		bool is_contradicting() const {
			return prune_visitor->is_contradicting;
		}
		void set_contradicting(bool value) {
			prune_visitor->is_contradicting = value;
		}

		DPLLPropMaps get_prop_maps() {
			return prop_maps;
		}
		const DPLLPropMaps get_prop_maps() const {
			return prop_maps;
		}

	};

}
