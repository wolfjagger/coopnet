#pragma once

#include "coopnet/graph/sat_graph_translator.h"
#include "coopnet/sat/visitor/visit.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/problem/problem.h"



namespace coopnet {

	// Note: Formula and children should have NO knowledge of Node
	//  and instead should rely on VertDescriptor

	template<typename SatProp>
	class Formula {

	protected:

		using Graph = SatGraph<SatProp>;
		using VertAssignment = std::map<VertDescriptor, bool>;

		std::reference_wrapper<const Problem> prob;

	public:

		explicit Formula(const Problem& prob);

		Formula(const Formula& other) = default;
		Formula& operator=(const Formula& other) = default;

		Formula(Formula&& other) = default;
		Formula& operator=(Formula&& other) = default;

		virtual ~Formula() = default;



		Assignment create_assignment() const;
		void set_assignment(const Assignment& assignment);

		virtual void reset() = 0;

		virtual bool is_SAT() const = 0;

		virtual const Graph& graph() const = 0;

		auto node_begin() const;
		auto node_end() const;



	protected:

		virtual const SatGraphTranslator& get_sat_graph_translator() const = 0;

		virtual VertAssignment create_vert_assignment() const = 0;
		virtual void set_vert_assignment(const VertAssignment& assignment) = 0;

		virtual Graph& graph() = 0;

	};

#include "formula.inl"

}
