#pragma once

#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/sat/visitor/sat_visitor.h"
#include "walk_prop.h"



namespace coopnet {

	struct WalkVisitor :
		public SatVertVisitor<WalkVisitor> {

		using event_filter = boost::on_examine_vertex;

		std::shared_ptr<size_t> numClausesFailed;



		explicit WalkVisitor(std::shared_ptr<size_t> numClausesFailed);



		void node_event(
			const WalkSatGraph& g, VertDescriptor node,
			const WalkVProp& prop);

		void clause_event(
			const WalkSatGraph& g, VertDescriptor clause,
			const WalkVProp& prop);

	};



	struct BfsWalkVisitor
		: public boost::bfs_visitor<WalkVisitor> {

		explicit BfsWalkVisitor(std::shared_ptr<size_t> pNumClausesFailed) :
			boost::bfs_visitor<WalkVisitor>(WalkVisitor(pNumClausesFailed)) {

		}

	};
}
