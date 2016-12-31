#include "random.h"
#include <algorithm>
#include <random>



namespace jj_random {
	
	std::random_device rd;
	auto engine = std::mt19937_64(rd());
	auto bern = std::bernoulli_distribution();
	auto uniform = std::uniform_real_distribution<double>();

	bool coin_flip() {
		return bern(engine);
	}
	
	unsigned rand_inclusive(unsigned min, unsigned max) {
		auto dist = std::uniform_int_distribution<unsigned>(min, max);
		return dist(engine);
	}

	std::vector<unsigned> rand_vec_less_than(
		unsigned num, unsigned vec_size, bool with_duplicates) {
		
		if(vec_size == 1) {

			auto vec = std::vector<unsigned>();
			vec.push_back(rand_less_than(num));
			return vec;

		} else {
			
			if (!with_duplicates && num < vec_size)
				throw std::exception("Random range too small for array.");

			auto prev_array = rand_vec_less_than(num, vec_size - 1, with_duplicates);
			auto new_num = rand_less_than(num);

			if(!with_duplicates) {
				constexpr auto escape_max = 20;
				auto escape = 0;
				while (std::any_of(prev_array.begin(), prev_array.end(),
					[new_num](int n) { return n == new_num; })) {
					new_num = rand_less_than(num);
					++escape;
					if (escape >= escape_max) throw std::exception("Couldn't find distinct nodes.");
				}
			}

			prev_array.push_back(new_num);

			return prev_array;

		}

	}


	double unit_rand() {
		return uniform(engine);
	}

}
