#pragma once

#pragma warning(push)
#pragma warning(disable: 4267)
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#pragma warning(pop)



using boost::undirected_graph;
using boost::graph_traits;

using boost::property;
using boost::no_property;
using boost::dynamic_properties;

using boost::vertex_color_t;
using boost::edge_weight_t;


using edge = std::pair<int, int>;
using edge3 = std::tuple<int, int, int>;

// vecS means std::vector, giving fast access and slow amendment
/*using Graph = adjacency_list<
	boost::vecS, boost::vecS, boost::undirectedS,
	property<vertex_color_t, std::string>,
	property<edge_weight_t, bool>>;*/
using graph = undirected_graph<
	property<vertex_color_t, std::string>,
	property<edge_weight_t, bool>>;
using vertex_descriptor = graph::vertex_descriptor;
