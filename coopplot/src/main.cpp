#include "net/graph_sat.h"



int main(int, char*[]) {

	auto foldername = std::string("tmp_folder");

	coopplot::graph_satisfiability(foldername, true);

	coopplot::graph_multi_satisfiability(foldername, true);

	return 0;

}
