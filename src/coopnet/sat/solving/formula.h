#pragma once

#include <functional>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "node_choice.h"



namespace sat {

	class problem;

	class formula {

	private:

		std::reference_wrapper<const problem> prob;

	protected:

		// Assignment information, including "unassigned"
		incomplete_assignment partial_assign;

	public:

		explicit formula(const problem& prob);

		formula(const formula& other) = default;
		formula& operator=(const formula& other) = default;

		formula(formula&& other) = default;
		formula& operator=(formula&& other) = default;

		~formula() = default;



		template<typename visitor>
		void visit_problem(visitor& v) {

			auto bfv = boost::make_bfs_visitor(v);

			auto sources = std::vector<size_t>();
			for (auto source_vert : get_prob_connected_component_entry_pts())
				sources.push_back(boost::vertex(source_vert, get_prob_graph()));

			auto buffer = boost::queue<vertex_descriptor>();

			using vec_color_type = std::vector<vertex_descriptor>;
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

			auto buffer = boost::queue<vertex_descriptor>();

			using vec_color_type = std::vector<vertex_descriptor>;
			auto vec_colors = vec_color_type(boost::num_vertices(get_prob_graph()));
			auto color_map = boost::make_iterator_property_map(
				vec_colors.begin(), get(boost::vertex_index, get_prob_graph()));

			boost::breadth_first_search(
				get_prob_graph(), sources.cbegin(), sources.cend(), buffer,
				bfv, color_map);

		}

		

		const incomplete_assignment& get_incomplete_assignment() const {
			return partial_assign;
		}
		incomplete_assignment& get_incomplete_assignment() {
			return partial_assign;
		}

		bool is_SAT() const;



	protected:

		const graph& get_prob_graph() const;

		const std::vector<vertex_descriptor>&
			get_prob_connected_component_entry_pts() const;

	};

}
