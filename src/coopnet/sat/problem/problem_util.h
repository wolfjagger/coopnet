#pragma once



namespace coopnet { namespace problem_util {

	size_t max_num_clauses_with_length(size_t num_nodes, size_t max_clause_length);

	size_t max_num_clauses_with_lengths(
		size_t num_nodes, size_t min_clause_length, size_t max_clause_length);

} }
