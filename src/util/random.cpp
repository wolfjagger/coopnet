#include "random.h"
#include <algorithm>
#include <random>



namespace jj_random {
	
	std::random_device rd;
	auto engine = std::mt19937_64(rd());
	auto uniform = std::uniform_real_distribution<double>();

	bool take_chance(double prob) {
		auto bern_prob = std::bernoulli_distribution(prob);
		return bern_prob(engine);
	}
	
	int rand_inclusive(int min, int max) {
		auto dist = std::uniform_int_distribution<unsigned>(min, max);
		return dist(engine);
	}

	std::vector<int> rand_vec_inclusive(
		int min, int max,
		unsigned vec_size, bool with_duplicates) {

		if (vec_size == 1) {

			auto vec = std::vector<int>();
			vec.push_back(rand_inclusive(min, max));
			return vec;

		} else {
			
			if (!with_duplicates && (max - min + 1) < int(vec_size))
				throw std::exception("Random range too small for array.");

			auto prev_array = rand_vec_inclusive(min, max, vec_size - 1, with_duplicates);
			auto new_num = rand_inclusive(min, max);

			if(!with_duplicates) {
				constexpr auto escape_max = 20;
				auto escape = 0;
				while (std::any_of(prev_array.begin(), prev_array.end(),
					[new_num](int n) { return n == new_num; })) {
					new_num = rand_inclusive(min, max);
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
