#pragma once

#include <map>
#include <memory>
#include "coopnet/graph/graph.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"



namespace sat {

	struct assignment;
	class node_shuffler;
	class sgn_shuffler;

	class problem {

	private:

		// We need a good storage solution that can put nodes and clauses
		//  small distance from eachother when they are accessed.
		//  e.g. we want the nodes in a clause stored relatively close to
		//  the clause. Perhaps along a semi-metric?

		// Cache number of nodes and clauses
		size_t num_nodes;
		size_t num_clauses;

		// Graph and properties
		graph prob_graph;
		boost::dynamic_properties dyn_props;

		// Map node with id to vertex_descriptor
		std::shared_ptr<node_vert_map> map_node_to_vert;

		// Connected components members
		size_t num_connected_components;
		std::vector<vertex_descriptor> connected_component_vertices;

	public:

		template<typename iterator>
		problem(size_t init_num_nodes,
			iterator clause_init_beg, iterator clause_init_end) {

			auto nodes = create_nodes(init_num_nodes);
			auto clauses = clause_list(clause_init_beg, clause_init_end);

			build_graph(std::move(nodes), std::move(clauses));
			
		}

		template<typename iterator>
		problem(iterator clause_init_beg, iterator clause_init_end) {

			auto init_num_nodes
				= std::max_element(clause_init_beg, clause_init_end,
				[](const clause& c) {
				return std::max_element(c.nodes().cbegin, c.nodes().cend());
			});

			auto nodes = create_nodes(init_num_nodes);
			auto clauses = clause_list(clause_init_beg, clause_init_end);

			build_graph(std::move(nodes), std::move(clauses));
			
		}



		// Should test if shared_ptr costs us.
		//  Would it be better to copy it in, or maybe just reference?
		clause_satisfiability clause_satisfiability_for(
			std::shared_ptr<const assignment> assign) const;

		size_t num_satisfied_by(std::shared_ptr<const assignment> assign) const {
			return clause_satisfiability_for(assign).clauses_satisfied.size();
		}
		size_t num_unsatisfied_by(std::shared_ptr<const assignment> assign) const {
			return num_clauses - num_satisfied_by(assign);
		}

		bool is_satisfied_by(std::shared_ptr<const assignment> assign) const {
			return num_unsatisfied_by(assign) == 0;
		}

		std::shared_ptr<assignment> create_same_sgn_assignment(bool sgn) const;



		const graph& get_graph() const { return prob_graph; }
		const boost::dynamic_properties& get_properties() const { return dyn_props; }

		std::shared_ptr<const node_vert_map> get_node_vert_map() const {
			return map_node_to_vert;
		}

		size_t get_num_connected_components() const {
			return num_connected_components;
		}
		const std::vector<vertex_descriptor>&
			connected_component_entry_pts() const {
			return connected_component_vertices;
		}

		auto get_num_nodes() const { return num_nodes; }
		auto get_num_clauses() const { return num_clauses; }
		auto get_num_verts() const { return num_nodes + num_clauses; }



		/*
		NOTE: This will also change it for satsifiability_visitor,
		incomplete_assignment, and anything that holds the same
		shared_ptr. Change them too!
		*/
		void shuffle_nodes(const node_shuffler& shuffler);
		void shuffle_sgns(const sgn_shuffler& shuffler);



	private:

		void build_graph(node_list&& nodes, clause_list&& clauses);

	};

	std::ostream& operator<<(std::ostream& os, const problem& prob);

}
