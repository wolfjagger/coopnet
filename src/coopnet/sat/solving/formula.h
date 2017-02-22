#pragma once

#include <functional>
#include "coopnet/sat/visitor/visit.h"
#include "coopnet/sat/problem/assignment.h"
#include "node_choice.h"



namespace sat {

	class Problem;

	class Formula {

	private:

		std::reference_wrapper<const Problem> prob;

	protected:

		// Assignment information, including "unassigned"
		PrunableSatGraph pruneGraph;

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
				sources.push_back(boost::vertex(source_vert, prob_graph()));

			visit_sat_graph(v, prob_graph(), sources.cbegin(), sources.cend());

		}

		template<typename PruneVisitor>
		void visit_active_graph(PruneVisitor& v) const {
			pruneGraph.visit(v);
		}

		

		void reverse_prune_to_assignment(Node n);



		const NodeVertMap& node_vert_map() const;
		std::shared_ptr<const NodeVertMap> node_vert_map_ptr() const;

		const PrunableSatGraph& prune_graph() const;

		bool is_SAT() const;
		Assignment create_assignment() const;



		const SatGraph& prob_graph() const;

	};

}
