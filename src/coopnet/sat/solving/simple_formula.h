#pragma once

#include "formula.h"



namespace coopnet {

	class SimpleFormula : public Formula<BaseSatProp> {

	private:

		Graph g;

	public:

		explicit SimpleFormula(const Problem& prob) :
			Formula<BaseSatProp>(prob),
			g(prob.get_graph()) { }



		const Graph& graph() const override { return g; }

	protected:

		Graph& graph() override { return g; }

	};

}
