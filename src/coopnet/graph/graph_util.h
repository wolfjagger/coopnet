#pragma once

#include "boost/graph/connected_components.hpp"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"
#include "graph.h"



namespace coopnet { namespace graph_util {

	inline std::string node_name(Node n) {

		auto name = std::string("n");
		name += std::to_string(n.id);
		return name;

	}

	inline std::string clause_name(const Clause& c) {

		auto name = std::string("c");
		for (auto lit : c.literals()) {
			name += lit.second ? "p" : "n";
			name += std::to_string(lit.first.id);
		}
		return name;

	}

	template<typename SatGraph>
	void rename_verts(
		SatGraph& g, const NodeVertMap& node_to_vertex_map) {

		auto vert_pair = boost::vertices(g);
		for (auto vert_iter = vert_pair.first;
			vert_iter != vert_pair.second; ++vert_iter) {

			auto vert = *vert_iter;

			switch (g[vert].base.kind) {
			case BaseSatVProp::Node: {

				auto n = node_to_vertex_map.right.at(vert);
				g[vert].base.name = node_name(n);

				break;

			}
			case BaseSatVProp::Clause: {

				auto lits = Clause::LitStorage();
				auto edge_pair = boost::out_edges(vert, g);
				for (auto edge_iter = edge_pair.first;
					edge_iter != edge_pair.second; ++edge_iter) {

					auto n = node_to_vertex_map.right.at(
						boost::target(*edge_iter, g));
					lits.emplace(std::make_pair(n, g[*edge_iter].base.sgn));

				}

				auto c = Clause(lits);
				g[vert].base.name = clause_name(c);

				break;

			}
			}

		}

	}



	template<typename SatGraph>
	std::vector<VertDescriptor>
		calculate_connected_components(const SatGraph& prob_graph) {

		using conn_map_type = std::map<VertDescriptor, size_t>;
		auto connected_map = conn_map_type();
		auto boost_conn_map =
			boost::associative_property_map<conn_map_type>(connected_map);
		auto num_connected_components
			= boost::connected_components(prob_graph, boost_conn_map);

		auto connected_component_vertices = std::vector<VertDescriptor>();
		connected_component_vertices.reserve(num_connected_components);
		auto set_done_components = std::set<size_t>();
		for (auto vert_pair = boost::vertices(prob_graph);
			vert_pair.first != vert_pair.second; ++vert_pair.first) {

			auto component_idx = connected_map[*vert_pair.first];
			if (set_done_components.count(component_idx) == 0) {
				set_done_components.insert(component_idx);
				connected_component_vertices.emplace_back(*vert_pair.first);
			}

		}

		return std::move(connected_component_vertices);

	}

}}
