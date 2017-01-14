#pragma once

#include "sat/graph.h"



namespace sat {

	struct assignment {

		using map = std::map<vertex_descriptor, bool>;
		
		map data;

		/*boost::associative_property_map<map> property_map() {
			return data;
		}*/

	};

}
