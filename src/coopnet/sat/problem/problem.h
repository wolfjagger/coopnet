#pragma once

#include <map>
#include <memory>
#include "coopnet/graph/sat_graph_translator.h"
#include "coopnet/sat/component/component_fwd.h"
#include "shuffler.h"



namespace coopnet {

	struct Assignment;

	class Problem {

	private:

		// We need a good storage solution that can put nodes and clauses
		//  small distance from eachother when they are accessed.
		//  e.g. we want the nodes in a clause stored relatively close to
		//  the clause. Perhaps along a semi-metric?

		// Cache number of nodes and clauses
		size_t numNodes;
		size_t numClauses;

		// Graph and properties
		BaseSatGraph probGraph;
		std::shared_ptr<SatGraphTranslator> translator;
		boost::dynamic_properties dynProps;

	public:

		template<typename Iterator>
		Problem(size_t initNumNodes,
			Iterator clauseInitBeg, Iterator clauseInitEnd) {

			auto nodes = create_nodes(initNumNodes);
			auto clauses = ClauseList(clauseInitBeg, clauseInitEnd);

			build_graph(std::move(nodes), std::move(clauses));
			
		}

		template<typename Iterator>
		Problem(Iterator clauseInitBeg, Iterator clauseInitEnd) {

			auto initNumNodes
				= std::max_element(clauseInitBeg, clauseInitEnd,
				[](const Clause& c) {
				return std::max_element(c.nodes().cbegin, c.nodes().cend());
			});

			auto nodes = create_nodes(initNumNodes);
			auto clauses = ClauseList(clauseInitBeg, clauseInitEnd);

			build_graph(std::move(nodes), std::move(clauses));
			
		}



		// Should test if shared_ptr costs us.
		//  Would it be better to copy it in, or maybe just reference?
		ClauseSatisfiability clause_satisfiability_for(
			std::shared_ptr<const Assignment> assign) const;

		size_t num_satisfied_by(std::shared_ptr<const Assignment> assign) const {
			return clause_satisfiability_for(assign).clausesSatisfied.size();
		}
		size_t num_unsatisfied_by(std::shared_ptr<const Assignment> assign) const {
			return numClauses - num_satisfied_by(assign);
		}

		bool is_satisfied_by(std::shared_ptr<const Assignment> assign) const {
			return num_unsatisfied_by(assign) == 0;
		}

		std::shared_ptr<Assignment> create_same_sgn_assignment(bool sgn) const;



		const BaseSatGraph& get_graph() const { return probGraph; }
		const boost::dynamic_properties& get_properties() const { return dynProps; }

		std::shared_ptr<const SatGraphTranslator> get_node_vert_translator() const {
			return translator;
		}

		auto get_num_nodes() const { return numNodes; }
		auto get_num_clauses() const { return numClauses; }
		auto get_num_verts() const { return numNodes + numClauses; }



		friend void NodeShuffler::apply_to_problem(Problem&) const;
		friend void SgnShuffler::apply_to_problem(Problem&) const;



	private:

		void build_graph(NodeList&& nodes, ClauseList&& clauses);

	};

	std::ostream& operator<<(std::ostream& os, const Problem& prob);

}
