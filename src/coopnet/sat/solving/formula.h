#pragma once

#include <functional>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/problem/problem.h"



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

		virtual ~formula() = 0;



		const graph& get_prob_graph() const;

		template<typename visitor>
		void visit_problem(visitor& v) {
			prob.get().apply_visitor(v);
		}

		template<typename visitor>
		void visit_problem(const visitor& v) const {
			prob.get().apply_visitor(v);
		}

		

		const incomplete_assignment& get_incomplete_assignment() const {
			return partial_assign;
		}
		incomplete_assignment& get_incomplete_assignment() {
			return partial_assign;
		}

		bool is_SAT() const;

	};

}
