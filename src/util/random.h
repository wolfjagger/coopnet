#pragma once

#include <vector>



namespace jj_random {
	
	// Boolean

	// 50/50 coin flip
	bool coin_flip();


	// Integer

	// Random integer btwn min and max, inclusive
	unsigned rand_inclusive(unsigned min, unsigned max);
	
	// Random integer in [0, num)
	inline unsigned rand_less_than(unsigned int num) {
		return rand_inclusive(0, int(num - 1));
	}


	// Int arrays

	// A vector of size vec_size with random numbers in [0, num),
	//  with or without possible duplicates
	std::vector<unsigned> rand_vec_less_than(
		unsigned num, unsigned vec_size, bool with_duplicates = false);


	// Double
	
	// Random number btwn 0 and 1
	double unit_rand();

	// Random number btwn min and max
	inline double rand_btwn(double min, double max) {
		return min + unit_rand()*max;
	}

}
