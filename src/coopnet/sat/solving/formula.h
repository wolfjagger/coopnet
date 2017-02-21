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
		IncompleteAssignment partial_assign;

	public:

		explicit Formula(const Problem& prob);

		Formula(const Formula& other) = default;
		Formula& operator=(const Formula& other) = default;

		Formula(Formula&& other) = default;
		Formula& operator=(Formula&& other) = default;

		~Formula() = default;



		template<typename Visitor>
		void visit_problem(const Visitor& v) const {

			auto sources = std::vector<size_t>();
			for (auto source_vert : get_prob_connected_component_entry_pts())
				sources.push_back(boost::vertex(source_vert, get_prob_graph()));

			visit_sat_graph(v, get_prob_graph(), sources.cbegin(), sources.cend());

		}

		

		const IncompleteAssignment& get_incomplete_assignment() const {
			return partial_assign;
		}
		IncompleteAssignment& get_incomplete_assignment() {
			return partial_assign;
		}

		bool is_SAT() const;



	protected:

		const SatGraph& get_prob_graph() const;

		const std::vector<VertDescriptor>&
			get_prob_connected_component_entry_pts() const;

	};

}
