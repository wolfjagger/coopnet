#include "problem_util.h"
#include <map>
#include <numeric>
#include <tuple>
#include "alphali/util/math.h"



namespace coopnet { namespace problem_util {

	namespace {

		std::map<std::pair<size_t, size_t>, size_t>
			max_num_cache_fixed_length;
		std::map<std::tuple<size_t, size_t, size_t>, size_t>
			max_num_cache_minmax_length;

	}



	size_t max_num_clauses_with_length(size_t num_nodes, size_t clause_length) {

		auto pair = std::make_pair(num_nodes, clause_length);
		if (max_num_cache_fixed_length.count(pair)) {
			return max_num_cache_fixed_length[pair];
		} else {
			auto max_num_clauses
				= alphali::nchoosek(num_nodes, clause_length) * alphali::powtwo(clause_length);
			max_num_cache_fixed_length.emplace(pair, max_num_clauses);
			return max_num_clauses;
		}

	}

	size_t max_num_clauses_with_lengths(
		size_t num_nodes, size_t min_clause_length, size_t max_clause_length) {

		if (min_clause_length == max_clause_length)
			return max_num_clauses_with_length(num_nodes, min_clause_length);

		if (min_clause_length < max_clause_length)
			throw std::runtime_error("min_clause length < max_clause_length.");

		auto tuple = std::make_tuple(num_nodes, min_clause_length, max_clause_length);
		if (max_num_cache_minmax_length.count(tuple)) {
			return max_num_cache_minmax_length[tuple];
		} else {

			auto max_len_num_clauses
				= max_num_clauses_with_length(num_nodes, max_clause_length);

			auto max_num_clauses
				= max_len_num_clauses
				+ max_num_clauses_with_lengths(
					num_nodes, min_clause_length, max_clause_length - 1);

			max_num_cache_minmax_length.emplace(tuple, max_num_clauses);
			return max_num_clauses;

		}

	}

} }
