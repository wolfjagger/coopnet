#pragma once

#include <string>



namespace coopplot {

	void graph_satisfiability(
		const std::string& foldername, bool withTime = true);

	void graph_multi_satisfiability(
		const std::string& foldername, bool withTime = true);

}
