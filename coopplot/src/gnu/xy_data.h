#pragma once

#include <array>
#include <vector>
#include "boost/variant.hpp"



namespace coopplot {

	template<class DataType>
	using RangeData = std::array<DataType, 2>;

	template<typename XType, typename YType>
	using FixedX = std::pair<RangeData<XType>, std::vector<YType>>;

	template<typename XType, typename YType>
	using FixedXWErr
		= std::pair<RangeData<XType>, std::vector<std::array<YType, 2>>>;

	template<typename XType, typename YType>
	using FixedXMultiY
		= std::pair<RangeData<XType>, std::vector<std::vector<YType>>>;

	template<typename XType, typename YType>
	using FixedXMultiYWErr
		= std::pair<RangeData<XType>,
		std::vector<std::vector<std::array<YType, 2>>>>;

	template<typename XType, typename YType>
	using VariedX = std::vector<std::pair<XType, YType>>;

	template<typename XType, typename YType>
	using VariedXWErr
		= std::vector<std::pair<XType, std::array<YType, 2>>>;

	template<typename XType, typename YType>
	using VariedXMultiY = std::vector<std::pair<XType, std::vector<YType>>>;

	template<typename XType, typename YType>
	using VariedXMultiYWErr
		= std::vector<std::pair<XType, std::vector<std::array<YType, 2>>>>;



	template<typename XType, typename YType>
	struct XYData {

		boost::variant<
			FixedX<XType, YType>,
			FixedXWErr<XType, YType>,
			FixedXMultiY<XType, YType>,
			FixedXMultiYWErr<XType, YType>,
			VariedX<XType, YType>,
			VariedXWErr<XType, YType>,
			VariedXMultiY<XType, YType>,
			VariedXMultiYWErr<XType, YType >> payload;

		template<typename SpecificType>
		XYData(SpecificType init_payload) :
			payload(init_payload) {

		}

	};

}
