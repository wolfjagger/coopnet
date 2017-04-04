#pragma once

#include "formula.h"



namespace coopnet {

	class SimpleFormula : public Formula<BaseSatVProp, BaseSatEProp> {

	private:

		Graph g;

	public:

		explicit SimpleFormula(const Problem& prob) :
			Formula<BaseSatVProp, BaseSatEProp>(prob),
			g(prob.get_graph()) { }



		const Graph& graph() const override { return g; }

	protected:

		Graph& graph() override { return g; }

	};

}
