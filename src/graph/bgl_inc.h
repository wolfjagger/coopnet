#pragma once

#pragma warning(push)
#pragma warning(disable: 4267)
#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#pragma warning(pop)



using boost::adjacency_list;
using boost::graph_traits;

using boost::property;
using boost::no_property;
using boost::dynamic_properties;

using boost::add_vertex;
using boost::add_edge;

using edge = std::pair<int, int>;
using edge3 = std::tuple<int, int, int>;
