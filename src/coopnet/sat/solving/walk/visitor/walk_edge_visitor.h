#pragma once

#include "alphali/designs/pubsub/collaborator.h"
#include "coopnet/sat/solving/walk/walk_prop.h"
#include "coopnet/sat/visitor/sat_visitor.h"



namespace coopnet {

	class WalkEdgeVisitor :
		public SatEdgeVisitor<WalkEdgeVisitor> {

	public:
		using event_filter = boost::on_examine_edge;

	public:

		WalkEdgeVisitor();

		void edge_event(
			const WalkSatGraph& g, EdgeDescriptor edge,
			const WalkEProp& prop,
			VertDescriptor node, VertDescriptor clause);

	private:

		void change_vert_status(
			VertDescriptor vert, const WalkVProp& prop, WalkVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, const WalkEProp& prop, WalkEdgeStatus new_status);

	};

}
