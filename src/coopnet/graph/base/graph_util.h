#pragma once

#include "boost/graph/connected_components.hpp"
#include "coopnet/graph/sat_graph_translator.h"
#include "coopnet/sat/component/node.h"
#include "coopnet/sat/component/clause.h"



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
		SatGraph& g, const SatGraphTranslator& translator) {

		auto vert_pair = boost::vertices(g);
		for (auto vert_iter = vert_pair.first;
			vert_iter != vert_pair.second; ++vert_iter) {

			auto vert = *vert_iter;

			switch (g[vert].kind) {
			case VertKind::Node: {

				auto n = translator.vert_to_node(vert);
				g[vert].name = node_name(n);

				break;

			}
			case VertKind::Clause: {

				auto lits = Clause::LitStorage();
				auto edge_pair = boost::out_edges(vert, g);
				for (auto edge_iter = edge_pair.first;
					edge_iter != edge_pair.second; ++edge_iter) {

					auto n = translator.vert_to_node(
						boost::target(*edge_iter, g));
					lits.emplace(std::make_pair(n, g[*edge_iter].sgn));

				}

				auto c = Clause(lits);
				g[vert].name = clause_name(c);

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



	template<typename SatProp>
	TranslatedSatGraph<SatProp> create_default_concrete_graph(
		const BaseSatGraph& baseGraph, const SatGraphTranslator& baseTranslator) {

		auto concreteGraph = SatGraph<SatProp>();
		auto nodeVertMap = NodeVertMap();

		auto vPair = boost::vertices(baseGraph);
		auto ePair = boost::edges(baseGraph);

		std::for_each(vPair.first, vPair.second,
			[&concreteGraph, &nodeVertMap, &baseGraph, &baseTranslator](VertDescriptor v) {

			auto prop = copy_base<SatProp>(baseGraph[v]);

			auto newVert = boost::add_vertex(prop, concreteGraph);

			if(VertKind(prop.kind) == VertKind::Node) {
				auto node_to_add = baseTranslator.vert_to_node(v);
				nodeVertMap.insert(NodeVertMap::value_type(node_to_add, newVert));
			}

		});

		std::for_each(ePair.first, ePair.second,
			[&concreteGraph, &baseGraph](EdgeDescriptor e) {

			auto prop = copy_base<SatProp>(baseGraph[e]);

			auto vert1 = boost::source(e, concreteGraph);
			auto vert2 = boost::target(e, concreteGraph);

			boost::add_edge(vert1, vert2, prop, concreteGraph);

		});

		return TranslatedSatGraph<SatProp> {
			std::move(concreteGraph),
			SatGraphTranslator(std::move(nodeVertMap))
		};

	}

}}
