#pragma once

#include "rapidcheck.h"
#include "coopnet/sat/problem.h"



namespace rc {

	template<>
	struct Arbitrary<sat::problem> {
		static Gen<sat::problem> arbitrary();
	};

}
