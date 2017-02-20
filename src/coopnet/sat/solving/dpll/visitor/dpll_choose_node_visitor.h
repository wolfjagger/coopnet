#pragma once

#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/visitor/sat_visitor.h"
#include "coopnet/sat/solving/dpll/dpll_prop_maps.h"



namespace sat {

	class DPLLChooseNodeVisitor :
		public SatVertVisitor<DPLLChooseNodeVisitor> {

	public:
		using event_filter = boost::on_examine_vertex;
		using IncompleteAssignmentPropMap
			= boost::associative_property_map<IncompleteAssignment::Map>;

	private:

		const DPLLPropMaps maps;

		std::shared_ptr<VertDescriptor> chosen_node;
		std::shared_ptr<bool> sgn_sat;

		int max_num_clauses_sat;

	public:

		DPLLChooseNodeVisitor(DPLLPropMaps init_maps);

		void node_event(
			const SatGraph& g, VertDescriptor node,
			const VertProp& prop);

		void clause_event(
			const SatGraph& g, VertDescriptor clause,
			const VertProp& prop);

		void reset();



		std::pair<VertDescriptor, bool> retreive_choice() const;

	};

}
