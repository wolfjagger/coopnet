#pragma once

#include "coopnet/sat/visitor/sat_visitor.h"
#include "coopnet/graph/graph.h"



namespace sat::test {

	struct MockSatVertVisitor
		: public SatVertVisitor<MockSatVertVisitor> {

		void node_event(
			const SatGraph& g, VertDescriptor node,
			const VertProp& prop) {

		}

		void clause_event(
			const SatGraph& g, VertDescriptor clause,
			const VertProp& prop) {

		}

	};



	class MockSatEdgeVisitor
		: public SatEdgeVisitor<MockSatEdgeVisitor> {

		void edge_event(
			const SatGraph& g, EdgeDescriptor edge,
			const EdgeProp& prop,
			VertDescriptor node, VertDescriptor clause) {

		}

	};

}
