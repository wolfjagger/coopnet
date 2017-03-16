#include "general.gen.h"
#include "alphali/util/random.h"



using namespace rc;

template<>
Gen<double> rc::gen::inRange<double>(double min, double max) {

	// BROKEN
	/*return gen::exec([=]() {

		if (max <= min) {
			std::string msg;
			msg += "Invalid range [" + std::to_string(min);
			msg += ", " + std::to_string(max) + ")";
			throw GenerationFailure(msg);
		}

		auto rand = alphali::rand_btwn(min, max);

		assert(rand >= min && rand <= max);
		return rand;

	});*/
	return gen::arbitrary<double>();

}
