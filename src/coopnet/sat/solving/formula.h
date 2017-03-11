#pragma once

#include <functional>
#include "coopnet/sat/visitor/visit.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/graph/mutable_graph.h"
#include "node_choice.h"



namespace coopnet {

	template<typename VProp, typename EProp>
	class Formula {

	private:

		using Graph = SatGraph<VProp, EProp>;
		using ExtendedGraph = ExtendedSatGraph<VProp, EProp>;

		std::reference_wrapper<const Problem> prob;

	protected:

		ExtendedGraph extendedGraph;

	public:

		explicit Formula(const Problem& prob);

		Formula(const Formula& other) = default;
		Formula& operator=(const Formula& other) = default;

		Formula(Formula&& other) = default;
		Formula& operator=(Formula&& other) = default;

		~Formula() = default;



		template<typename Visitor>
		void visit_entire_graph(const Visitor& v) const {

			auto sources = std::vector<size_t>();
			for (auto source_vert : pruneGraph.connected_component_entry_pts())
				sources.push_back(boost::vertex(source_vert, graph()));

			visit_sat_graph(v, graph(), sources.cbegin(), sources.cend());

		}

		template<typename PruneVisitor>
		void visit_active_graph(PruneVisitor& v) const {
			extendedGraph.visit(v);
		}

		

		void reverse_prune_to_assignment(Node n);



		const NodeVertMap& node_vert_map() const;
		std::shared_ptr<const NodeVertMap> node_vert_map_ptr() const;

		const ExtendedGraph& extended_graph() const;
		const Graph& graph() const;

		bool is_SAT() const;
		IncompleteAssignment create_incomplete_assignment() const;
		Assignment create_assignment() const;

	};

#include "formula.inl"

}
