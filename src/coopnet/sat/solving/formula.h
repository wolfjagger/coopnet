#pragma once

#include <functional>
#include "coopnet/sat/visitor/visit.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/problem/problem.h"



namespace coopnet {

	template<typename VProp, typename EProp>
	class Formula {

	protected:

		using Graph = SatGraph<VProp, EProp>;

		std::reference_wrapper<const Problem> prob;

	public:

		explicit Formula(const Problem& prob);

		Formula(const Formula& other) = default;
		Formula& operator=(const Formula& other) = default;

		Formula(Formula&& other) = default;
		Formula& operator=(Formula&& other) = default;

		~Formula() = default;



		const NodeVertMap& node_vert_map() const;
		std::shared_ptr<const NodeVertMap> node_vert_map_ptr() const;



		virtual bool is_SAT() const = 0;
		virtual Assignment create_assignment() const = 0;
		virtual void set_assignment(const Assignment& assignment) = 0;

		virtual const Graph& graph() const = 0;

	protected:

		template<typename Pred>
		void apply_to_node_vert_map(Pred&& pred) const;

		virtual Graph& graph() = 0;

	};

#include "formula.inl"

}
