#pragma once

#include "coopnet/sat/visitor/sat_visitor.h"
#include "coopnet/graph/graph.h"



namespace coopnet::test {

	struct MockSatVertVisitor
		: public SatVertVisitor<MockSatVertVisitor> {

		void node_event(
			const BaseSatGraph& g, VertDescriptor node,
			const BaseSatVProp& prop) {

		}

		void clause_event(
			const BaseSatGraph& g, VertDescriptor clause,
			const BaseSatVProp& prop) {

		}

	};



	class MockSatEdgeVisitor
		: public SatEdgeVisitor<MockSatEdgeVisitor> {

		void edge_event(
			const BaseSatGraph& g, EdgeDescriptor edge,
			const BaseSatEProp& prop,
			VertDescriptor node, VertDescriptor clause) {

		}

	};

}
