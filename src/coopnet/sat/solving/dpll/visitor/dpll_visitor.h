#pragma once

#include <queue>
#include "boost/graph/breadth_first_search.hpp"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/visitor/pruned_sat_visitor.h"
#include "coopnet/sat/solving/dpll/dpll_prop_maps.h"



namespace sat {

	// This visitor will decide if the vert should be removed:
	//  either if it only has one edge (clause)
	//  or if it has all edges with same sign (node).
	// It also needs to color the surrounding edges if they
	//  should be (re)visited (i.e. if vert is to be removed).
	class DPLLBeginVertVisitor :
		public PruneSatVertVisitor<DPLLBeginVertVisitor, PruneInfo> {
	
	public:
		using event_filter = boost::on_examine_vertex;
		
	private:

		bool& isContradicting;
		DPLLPropMaps maps;

	public:

		DPLLBeginVertVisitor(
			PruneInfo& initPruneInfo,
			bool& initIsContradicting,
			DPLLPropMaps initMaps);

		void node_event(
			const SatGraph& g, VertDescriptor node,
			const VertProp& prop);

		void clause_event(
			const SatGraph& g, VertDescriptor clause,
			const VertProp& prop);

	private:

		void select_node(const SatGraph& g, VertDescriptor node, bool sgn);
		void satisfy_clause(const SatGraph& g, VertDescriptor clause);

		void change_vert_status(
			VertDescriptor vert, DPLLVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, DPLLEdgeStatus new_status);

	};



	// This has three responsibilities.
	// (1) color unvisited vertex to visit it
	// (2) it needs to propagate removals to the next vertices,
	//  with cases depending on the circumstances:
	//  (a) node = a => remove clause iff sgn(edge) == a
	//  (b) clause => set node = sgn(edge) iff clause.num_edges == 1
	// (3) remove edge
	class DPLLEdgeVisitor :
		public PruneSatEdgeVisitor<DPLLEdgeVisitor, PruneInfo> {
	
	public:
		using event_filter = boost::on_examine_edge;

	private:

		bool& isContradicting;
		DPLLPropMaps maps;

	public:

		DPLLEdgeVisitor(
			PruneInfo& initPruneInfo,
			bool& initIsContradicting,
			DPLLPropMaps initMaps);

		void edge_event(
			const SatGraph& g, EdgeDescriptor edge,
			const EdgeProp& prop,
			VertDescriptor node, VertDescriptor clause);

	private:

		void change_vert_status(
			VertDescriptor vert, DPLLVertStatus new_status);
		void change_edge_status(
			EdgeDescriptor edge, DPLLEdgeStatus new_status);

	};


	// This visitor colors remaining verts grey and in the
	//  queue to black and pops the queue.
	class DPLLFinishVertVisitor :
		public boost::base_visitor<DPLLFinishVertVisitor> {
	
	public:
		using event_filter = boost::on_finish_vertex;

	private:
		boost::queue<VertDescriptor>& greyBuffer;
		bool& isContradicting;
		DPLLPropMaps maps;

	public:

		DPLLFinishVertVisitor(
			boost::queue<VertDescriptor>& initGreyBuffer,
			bool& initIsContradicting,
			DPLLPropMaps initMaps);

		template<class Vertex, class Graph>
		void operator()(Vertex v, Graph& g) {

			if (isContradicting) {
				while (!greyBuffer.empty()) {
					auto vert = greyBuffer.front();
					maps.colorMap[vert] = default_color_type::black_color;
					greyBuffer.pop();
				}
			}

		}

	};



	using DPLLVisitorTuple = 
		std::pair<DPLLBeginVertVisitor,
		std::pair<DPLLEdgeVisitor, DPLLFinishVertVisitor>>;



	class DPLLVisitor :
		public boost::bfs_visitor<DPLLVisitorTuple> {

	public:
		bool isContradicting;

	public:

		DPLLVisitor(
			PruneInfo& initPruneInfo,
			boost::queue<VertDescriptor>& greyQueue,
			DPLLPropMaps initMaps);

	};
	
}
