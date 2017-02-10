#pragma once

#include <vector>
#include <map>
#include <memory>
#include "coopnet/graph/graph.h"
#include "problem.h"



namespace sat {

	struct node;
	struct literal;
	struct assignment;
	class problem;



	class literal_shuffler {

	private:

		std::vector <literal> literals;
		std::shared_ptr<const node_vert_map> map_node_to_vert;

	public:

		// Note: shuffles on creation
		literal_shuffler(const problem& prob);



		void shuffle();


		bool flips_nodes() const;
		

		literal shuffle_literal(const literal& lit) const;

		void apply_to_assignment(assignment& assign) const;

		void apply_to_problem(problem& prob) const;



		friend void problem::apply_shuffle(const literal_shuffler&);

	};

}