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



	class i_shuffler {

	public:

		virtual void shuffle() = 0;


		virtual bool is_trivial() const = 0;


		virtual void apply_to_assignment(assignment& assign) const = 0;

		virtual void apply_to_problem(problem& prob) const = 0;

	};



	class node_shuffler : public i_shuffler {

	private:

		std::vector<node> nodes;
		std::shared_ptr<const node_vert_map> map_node_to_vert;

	public:

		// Note: shuffles on creation
		node_shuffler(const problem& prob);

		node_shuffler(const problem& prob, std::vector<node> shuffle_nodes);



		void shuffle() override;


		bool is_trivial() const override;


		node shuffled_node(node orig) const;


		void apply_to_assignment(assignment& assign) const override;

		void apply_to_problem(problem& prob) const override;



		friend void problem::shuffle_nodes(const node_shuffler&);

	};



	class sgn_shuffler : public i_shuffler {

	private:

		std::vector<bool> sgns;
		std::shared_ptr<const node_vert_map> map_node_to_vert;

	public:

		// Note: shuffles on creation
		sgn_shuffler(const problem& prob);

		sgn_shuffler(const problem& prob, std::vector<bool> shuffle_sgns);



		void shuffle() override;


		bool is_trivial() const override;


		bool shuffled_sgn(node n) const;


		void apply_to_assignment(assignment& assign) const override;

		void apply_to_problem(problem& prob) const override;



		friend void problem::shuffle_sgns(const sgn_shuffler&);

	};



	class literal_shuffler : public i_shuffler {

	private:

		node_shuffler node_sh;
		sgn_shuffler sgn_sh;

	public:

		// Note: shuffles on creation
		literal_shuffler(const problem& prob);

		literal_shuffler(
			const problem& prob, const std::vector<literal>& lits);



		void shuffle() override;


		bool is_trivial() const override;
		bool shuffles_nodes() const;
		bool flips_sgns() const;
		

		literal shuffled_literal(literal lit) const;


		void apply_to_assignment(assignment& assign) const override;

		void apply_to_problem(problem& prob) const override;

	};

}