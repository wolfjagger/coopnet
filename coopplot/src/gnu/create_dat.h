#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "net/sat_xy_data.h"



namespace coopplot {

	// Visitor to create dat file depending on value of variant "payload"
	template<typename x_type, typename y_type>
	struct create_dat : public boost::static_visitor<std::string> {

		template<typename payload_type>
		std::string operator()(
			payload_type payload) const;
		
		template<>
		std::string operator()
			<fixed_x<x_type, y_type>>
			(fixed_x<x_type, y_type> payload) const {

			auto stream = std::stringstream();
			stream << "#x y" << std::endl;

			auto x_diff = payload.first[1];
			auto x = payload.first[0];
			for (auto y : payload.second) {
				print_x_y(stream, x, y);
				x += x_diff;
			}

			return stream.str();

		}

		template<>
		std::string operator()
			<varied_x<x_type, y_type>>
			(varied_x<x_type, y_type> payload) const {

			auto stream = std::stringstream();
			stream << "#x y" << std::endl;

			for (auto pair : payload) {
				print_x_y(stream, pair.first, pair.second);
			}

			return stream.str();

		}

	private:

		static void print_x_y(std::stringstream& stream, x_type x, y_type y) {
			stream << x << "   " << y << std::endl;
		}

	};

}
