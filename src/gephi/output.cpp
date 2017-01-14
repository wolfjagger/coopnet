#include "output.h"
#include <fstream>
#include <string>
#include <boost/graph/graphml.hpp>



namespace jj_gephi {

	void print_graph(const sat::graph& g, const boost::dynamic_properties& dp) {

		// If file exists, choose other file
		std::string dirname = "out/";
		std::string filename = "gephi_try.graphml";
		if (std::ifstream(dirname + filename)) {

			auto i = 1;
			while(i < 10 && std::ifstream(dirname + filename)) {

				filename = "gephi_try_";
				filename += std::to_string(i);
				filename += ".graphml";

				++i;

			}

		}

		// Open file
		std::ofstream file;
		file.open(dirname + filename);
		boost::write_graphml(file, g, dp);
		file.close();

	}

}
