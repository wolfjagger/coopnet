#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/dynamic_property_map.hpp>



using boost::adjacency_list;
using boost::property;
using boost::vertex_color_t;
using boost::edge_weight_t;
using boost::dynamic_properties;
using boost::graph_traits;


using Edge = std::pair<int, int>;
using Edge3 = std::tuple<int, int, int>;

// vecS means std::vector, giving fast access and slow amendment
using Graph = adjacency_list<
	boost::vecS, boost::vecS, boost::undirectedS,
	property<vertex_color_t, std::string>,
	property<edge_weight_t, int>>;
