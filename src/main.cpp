#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <boost/lambda/lambda.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/dynamic_property_map.hpp>



enum files_e {
	dax_h, yow_h, boz_h, zow_h, foo_cpp,
	foo_o, bar_cpp, bar_o, libfoobar_a,
	zig_cpp, zig_o, zag_cpp, zag_o,
	libzigzag_a, killerapp, N
};
const char* name[] = { "dax.h", "yow.h", "boz.h", "zow.h", "foo.cpp",
"foo.o", "bar.cpp", "bar.o", "libfoobar.a",
"zig.cpp", "zig.o", "zag.cpp", "zag.o",
"libzigzag.a", "killerapp" };

int main(int, char*[])
{

	using namespace boost;

	// Open file
	std::ofstream file;
	file.open("gephi_try.graphml");

	typedef std::pair<int, int> Edge;
	Edge used_by[] = {
		Edge(dax_h, foo_cpp), Edge(dax_h, bar_cpp), Edge(dax_h, yow_h),
		Edge(yow_h, bar_cpp), Edge(yow_h, zag_cpp),
		Edge(boz_h, bar_cpp), Edge(boz_h, zig_cpp), Edge(boz_h, zag_cpp),
		Edge(zow_h, foo_cpp),
		Edge(foo_cpp, foo_o),
		Edge(foo_o, libfoobar_a),
		Edge(bar_cpp, bar_o),
		Edge(bar_o, libfoobar_a),
		Edge(libfoobar_a, libzigzag_a),
		Edge(zig_cpp, zig_o),
		Edge(zig_o, libzigzag_a),
		Edge(zag_cpp, zag_o),
		Edge(zag_o, libzigzag_a),
		Edge(libzigzag_a, killerapp)
	};

	const int nedges = sizeof(used_by) / sizeof(Edge);

	typedef adjacency_list< vecS, vecS, directedS,
		property< vertex_color_t, std::string >,
		property< edge_weight_t, int >
	> Graph;
	Graph g(used_by, used_by + nedges, N);

	graph_traits<Graph>::vertex_iterator v, v_end;
	for (boost::tie(v, v_end) = vertices(g); v != v_end; ++v)
		put(vertex_color_t(), g, *v, name[*v]);

	graph_traits<Graph>::edge_iterator e, e_end;
	for (boost::tie(e, e_end) = edges(g); e != e_end; ++e)
		put(edge_weight_t(), g, *e, 3);

	dynamic_properties dp;
	dp.property("name", get(vertex_color_t(), g));
	dp.property("weight", get(edge_weight_t(), g));

	write_graphml(file, g, dp, true);

	file.close();

	return 0;

}

/*int main()
{
	using namespace boost::lambda;
	typedef std::istream_iterator<float> in;

	std::for_each(
		in(std::cin), in(), std::cout << (_1 * 3) << " ");

}
*/
