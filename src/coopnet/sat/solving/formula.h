#pragma once

#include <functional>
#include "coopnet/graph/graph.h"
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



		template<typename visitor>
		void visit_problem(visitor& v) {

			auto bfv = boost::make_bfs_visitor(v);

			auto sources = std::vector<size_t>();
			for (auto source_vert : get_prob_connected_component_entry_pts())
				sources.push_back(boost::vertex(source_vert, get_prob_graph()));

			auto buffer = boost::queue<VertDescriptor>();

			using vec_color_type = std::vector<VertDescriptor>;
			auto vec_colors = vec_color_type(boost::num_vertices(get_prob_graph()));
			auto color_map = boost::make_iterator_property_map(
				vec_colors.begin(), get(boost::vertex_index, get_prob_graph()));

			boost::breadth_first_search(
				get_prob_graph(), sources.cbegin(), sources.cend(), buffer,
				bfv, color_map);

		}

		template<typename visitor>
		void visit_problem(const visitor& v) const {

			auto bfv = boost::make_bfs_visitor(v);

			auto sources = std::vector<size_t>();
			for (auto source_vert : get_prob_connected_component_entry_pts())
				sources.push_back(boost::vertex(source_vert, get_prob_graph()));

			auto buffer = boost::queue<VertDescriptor>();

			using vec_color_type = std::vector<VertDescriptor>;
			auto vec_colors = vec_color_type(boost::num_vertices(get_prob_graph()));
			auto color_map = boost::make_iterator_property_map(
				vec_colors.begin(), get(boost::vertex_index, get_prob_graph()));

			boost::breadth_first_search(
				get_prob_graph(), sources.cbegin(), sources.cend(), buffer,
				bfv, color_map);

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
