#pragma once

#include "alphali/containers/random_iterator.h"
#include "boost/logic/tribool.hpp"
#include "boost/optional.hpp"
#include "coopnet/graph/graph.h"
#include "coopnet/sat/problem/assignment.h"
#include "coopnet/sat/visitor/choose_max_clauses_visitor.h"
#include "node_choice.h"



namespace coopnet {

	template<class ConcreteFormula>
	class NodeChooser {

	protected:

		using VertChoice = std::pair<VertDescriptor, bool>;

	public:

		boost::optional<NodeChoice> choose(const ConcreteFormula& form);

	protected:

		virtual VertChoice do_choose(const ConcreteFormula& form) = 0;

	};





	template<class ConcreteFormula>
	class NextNodeChooser : public NodeChooser<ConcreteFormula> {

	protected:

		VertChoice do_choose(const ConcreteFormula& form) override;

	};


	template<class ConcreteFormula>
	class RandNodeChooser : public NodeChooser<ConcreteFormula> {

	protected:

		VertChoice do_choose(const ConcreteFormula& form) override;

	};



	template<class ConcreteFormula>
	class MaxSameClauseNodeChooser : public NodeChooser<ConcreteFormula> {

	protected:

		VertChoice do_choose(const ConcreteFormula& form) override;

	};

	template<class ConcreteFormula>
	class MaxTotClauseNodeChooser : public NodeChooser<ConcreteFormula> {

	protected:

		VertChoice do_choose(const ConcreteFormula& form) override;

	};



#include "node_chooser.inl"

}
