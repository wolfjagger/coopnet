#pragma once

#include <functional>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"



namespace sat {

	class problem;

	class formula {

	protected:

		std::reference_wrapper<const graph> prob_graph;

		// Assignment information, including "unassigned"
		incomplete_assignment partial_assign;

	public:

		explicit formula(const problem& prob);

		formula(const formula& other) = default;
		formula& operator=(const formula& other) = default;

		formula(formula&& other) = default;
		formula& operator=(formula&& other) = default;

		virtual ~formula() = 0;



		const incomplete_assignment& get_incomplete_assignment() const {
			return partial_assign;
		}
		incomplete_assignment& get_incomplete_assignment() {
			return partial_assign;
		}

		bool is_SAT() const;

	};

}
