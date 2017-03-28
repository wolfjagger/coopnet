#pragma once

#include "formula.h"



namespace coopnet {

	class SimpleFormula : public Formula<PruneSatVProp, PruneSatEProp> {

	public:

		explicit SimpleFormula(const Problem& prob) :
			Formula<PruneSatVProp, PruneSatEProp>(prob) { }

	};

}
