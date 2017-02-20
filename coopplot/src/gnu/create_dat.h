#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "xy_data.h"



namespace coopplot {

	// Visitor to create dat file depending on value of variant "payload"
	template<typename XType, typename YType>
	struct CreateDat : public boost::static_visitor<std::string> {

		template<typename PayloadType>
		std::string operator()(
			PayloadType payload) const;
		
		template<>
		std::string operator()
			<FixedX<XType, YType>>
			(FixedX<XType, YType> payload) const {

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
			< FixedXMultiY<XType, YType> >
			(FixedXMultiY<XType, YType> payload) const {

			auto stream = std::stringstream();
			stream << "#x y" << std::endl;

			auto x_diff = payload.first[1];
			auto x = payload.first[0];
			for (auto vec_y : payload.second) {
				print_x_y(stream, x, vec_y);
				x += x_diff;
			}

			return stream.str();

		}


		template<>
		std::string operator()
			<VariedX<XType, YType>>
			(VariedX<XType, YType> payload) const {

			auto stream = std::stringstream();
			stream << "#x y" << std::endl;

			for (auto pair : payload) {
				print_x_y(stream, pair.first, pair.second);
			}

			return stream.str();

		}

		template<>
		std::string operator()
			< VariedXMultiY<XType, YType> >
			(VariedXMultiY<XType, YType> payload) const {

			auto stream = std::stringstream();
			stream << "#x y" << std::endl;

			for (auto pair : payload) {
				print_x_y(stream, pair.first, pair.second);
			}

			return stream.str();

		}


	private:

		static void print_x_y(std::stringstream& stream, XType x, YType y) {
			stream << x << "   " << y << std::endl;
		}

		static void print_x_y(std::stringstream& stream, XType x, std::vector<YType> vec_y) {
			stream << x;
			for (auto y : vec_y) stream << "   " << y;
			stream << std::endl;
		}

	};



	template<typename XType, typename YType>
	struct CalcNumYCols : public boost::static_visitor<size_t> {

		template<typename PayloadType>
		size_t operator()(
			PayloadType payload) const;

		template<>
		size_t operator()
			< FixedX<XType, YType> >
			(FixedX<XType, YType> payload) const {
			return 1;
		}

		template<>
		size_t operator()
			< FixedXMultiY<XType, YType> >
			(FixedXMultiY<XType, YType> payload) const {
			return payload.second.at(0).size();
		}

		template<>
		size_t operator()
			< VariedX<XType, YType> >
			(VariedX<XType, YType> payload) const {
			return 1;
		}

		template<>
		size_t operator()
			< VariedXMultiY<XType, YType> >
			(VariedXMultiY<XType, YType> payload) const {
			return payload.at(0).second.size();
		}

	};
	
}
