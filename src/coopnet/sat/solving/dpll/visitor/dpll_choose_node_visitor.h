#pragma once

#include "coopnet/sat/visitor/pruned_sat_visitor.h"



namespace coopnet {

	class DPLLChooseNodeVisitor :
		public PruneSatVertVisitor<DPLLChooseNodeVisitor> {

	public:
		using event_filter = boost::on_examine_vertex;

	private:

		std::shared_ptr<VertDescriptor> chosen_node;
		std::shared_ptr<bool> sgn_sat;

		int max_num_clauses_sat;

	public:

		void node_event(
			const BaseSatGraph& g, VertDescriptor node,
			const BaseSatVProp& prop);

		void clause_event(
			const BaseSatGraph& g, VertDescriptor clause,
			const BaseSatVProp& prop);

		void reset();



		std::pair<VertDescriptor, bool> retreive_choice() const;

	};

}
