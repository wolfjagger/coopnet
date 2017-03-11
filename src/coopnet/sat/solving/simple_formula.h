#pragma once

#include "formula.h"



namespace coopnet {

	class SimpleFormula : public Formula<MutableSatVProp, MutableSatEProp> {

	public:

		explicit SimpleFormula(const Problem& prob) :
			Formula<MutableSatVProp, MutableSatEProp>(prob) { }

	};

}
