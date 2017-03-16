#pragma once

#include "rapidcheck.h"



namespace rc { namespace gen {

	template<>
	Gen<double> inRange<double>(double min, double max);

} }
