#pragma once

#include <vector>
#include <map>
#include <memory>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/component/component_fwd.h"



namespace coopnet {

	struct Assignment;
	class Problem;



	class IShuffler {

	public:

		virtual void shuffle() = 0;


		virtual bool is_trivial() const = 0;


		virtual void apply_to_assignment(Assignment& assign) const = 0;

		virtual void apply_to_problem(Problem& prob) const = 0;

	};



	class NodeShuffler : public IShuffler {

	private:

		std::vector<Node> nodes;
		std::shared_ptr<const NodeVertMap> map_node_to_vert;

	public:

		// Note: shuffles on creation
		NodeShuffler(const Problem& prob);

		NodeShuffler(const Problem& prob, std::vector<Node> shuffle_nodes);



		void shuffle() override;


		bool is_trivial() const override;


		Node shuffled_node(Node orig) const;


		void apply_to_assignment(Assignment& assign) const override;

		void apply_to_problem(Problem& prob) const override;

	};



	class SgnShuffler : public IShuffler {

	private:

		std::vector<bool> sgns;
		std::shared_ptr<const NodeVertMap> map_node_to_vert;

	public:

		// Note: shuffles on creation
		SgnShuffler(const Problem& prob);

		SgnShuffler(const Problem& prob, std::vector<bool> shuffle_sgns);



		void shuffle() override;


		bool is_trivial() const override;


		bool shuffled_sgn(Node n) const;


		void apply_to_assignment(Assignment& assign) const override;

		/*
		NOTE: This will also change it for satsifiability_visitor,
		incomplete_assignment, and anything that holds the same
		shared_ptr. Change them too!
		*/
		void apply_to_problem(Problem& prob) const override;

	};



	class LiteralShuffler : public IShuffler {

	private:

		NodeShuffler node_sh;
		SgnShuffler sgn_sh;

	public:

		// Note: shuffles on creation
		LiteralShuffler(const Problem& prob);

		LiteralShuffler(
			const Problem& prob, const std::vector<Literal>& lits);



		void shuffle() override;


		bool is_trivial() const override;
		bool shuffles_nodes() const;
		bool flips_sgns() const;
		

		Literal shuffled_literal(Literal lit) const;


		void apply_to_assignment(Assignment& assign) const override;

		/*
		NOTE: This will also change it for satsifiability_visitor,
		incomplete_assignment, and anything that holds the same
		shared_ptr. Change them too!
		*/
		void apply_to_problem(Problem& prob) const override;

	};

}