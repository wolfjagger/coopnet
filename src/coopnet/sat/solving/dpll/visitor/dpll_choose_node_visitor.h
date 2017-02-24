#pragma once

#include "coopnet/sat/visitor/pruned_sat_visitor.h"



namespace coopnet {

	class DPLLChooseNodeVisitor :
		public PruneSatVertVisitor<DPLLChooseNodeVisitor, const PruneInfo> {

	public:
		using event_filter = boost::on_examine_vertex;

	private:

		std::shared_ptr<VertDescriptor> chosen_node;
		std::shared_ptr<bool> sgn_sat;

		int max_num_clauses_sat;

	public:

		DPLLChooseNodeVisitor(const PruneInfo& initPruneInfo);

		void node_event(
			const BaseSatGraph& g, VertDescriptor node,
			const VertProp& prop);

		void clause_event(
			const BaseSatGraph& g, VertDescriptor clause,
			const VertProp& prop);

		void reset();



		std::pair<VertDescriptor, bool> retreive_choice() const;

	};

}
