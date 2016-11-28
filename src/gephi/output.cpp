#include "output.h"
#include <fstream>
#include <string>
#include <boost/graph/graphml.hpp>



namespace Gephi {

	void printGraph(const Graph& graph, const dynamic_properties& dp) {

		// If file exists, choose other file
		std::string filename = "gephi_try.graphml";
		if (std::ifstream(filename)) {

			auto i = 1;
			while(i < 10 && std::ifstream(filename)) {

				filename = "gephi_try_";
				filename += std::to_string(i);
				filename += ".graphml";

				++i;

			}

		}

		// Open file
		std::ofstream file;
		file.open(filename);
		boost::write_graphml(file, graph, dp, true);
		file.close();

	}

}
