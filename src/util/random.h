#pragma once

#include <vector>



namespace jj_random {
	
	// Boolean

	bool coin_flip();


	// Integer

	unsigned rand_inclusive(unsigned min, unsigned max);
	
	inline unsigned rand_less_than(unsigned int num) {
		return rand_inclusive(0, int(num - 1));
	}


	// Int arrays

	std::vector<unsigned> rand_vec_less_than(
		unsigned num, unsigned vec_size, bool with_duplicates = false);


	// Double
	
	double unit_rand();

	inline double rand_btwn(double min, double max) {
		return min + unit_rand()*max;
	}

}
