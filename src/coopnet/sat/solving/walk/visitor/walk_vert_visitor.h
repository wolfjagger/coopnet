#pragma once

#include "alphali/designs/pubsub/collaborator.h"
#include "coopnet/sat/solving/walk/walk_prop.h"
#include "coopnet/sat/visitor/sat_visitor.h"



namespace coopnet {

	class WalkVertVisitor :
		public SatVertVisitor<WalkVertVisitor> {

	public:
		using event_filter = boost::on_examine_vertex;

	public:

		WalkVertVisitor();

		void node_event(
			const WalkSatGraph& g, VertDescriptor node,
			const WalkVProp& prop);

		void clause_event(
			const WalkSatGraph& g, VertDescriptor clause,
			const WalkVProp& prop);

	private:

		void select_node(const WalkSatGraph& g,
			VertDescriptor node, const WalkVProp& prop, bool sgn);
		void satisfy_clause(const WalkSatGraph& g,
			VertDescriptor clause, const WalkVProp& prop);

		void change_vert_status(
			VertDescriptor vert, const WalkVProp& prop, WalkVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, const WalkEProp& prop, WalkEdgeStatus new_status);
		
	};

}
