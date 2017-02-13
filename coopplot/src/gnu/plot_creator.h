#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include "net/sat_xy_data.h"
#include "create_dat.h"



namespace coopplot {

	template<class data_type> using range_data = std::array<data_type, 2>;

	template<typename x_type, typename y_type>
	class plot_creator {

	private:

		static constexpr char* header = "gnuplot -p -e \"plot";
		static constexpr char* sep = " ";
		static constexpr char* footer = "\"";
		static constexpr char* gnu_quote = "\\\"";

		std::string x_range_str = "";
		std::string y_range_str = "";
		std::string options_str = "with lines";

		std::unique_ptr<std::string> p_filename;

	public:

		plot_creator() = default;

		plot_creator(range_data<x_type> x_range, range_data<y_type> y_range) {

			set_x_range(x_range);
			set_y_range(y_range);
			
		}



		// Set min and max x; default chosen by gnuplot
		void set_x_range(range_data<x_type> x_range) {
			x_range_str = "[" + std::to_string(x_range[0]);
			x_range_str += " to ";
			x_range_str += std::to_string(x_range[1]) + "]";
		}

		// Set min and max y; default chosen by gnuplot
		void set_y_range(range_data<y_type> y_range) {
			y_range_str = "[" + std::to_string(y_range[0]);
			y_range_str += " to ";
			y_range_str += std::to_string(y_range[1]) + "]";
		}

		// Add options; defaults to "with lines"
		void set_options(std::string options) {
			options_str = std::move(options);
		}



		// This creates the dat file that will be plotted.
		//  File must be created before gnuplot_str is called.
		void create_dat_file(
			sat_xy_data<x_type, y_type> sat_data,
			std::string filename) {

			p_filename
				= std::make_unique<std::string>(std::move(filename));

			auto file_str = boost::apply_visitor(
				create_dat<double, double>(),
				sat_data.payload);

			auto file = std::ofstream(*p_filename);

			file << file_str;

		}

		// Formats the string for calling gnuplot to plot the data.
		//  Dat file must be created before this call (throws otherwise).
		std::string gnuplot_str() const {

			if (p_filename) {

				auto str = std::string();
				str += header;
				str += sep;
				str += x_range_str;
				str += sep;
				str += y_range_str;
				str += sep;
				str += gnu_quote + *p_filename + gnu_quote;
				str += sep;
				str += options_str;
				str += footer;

				return str;

			} else {
				throw std::exception(
					"gnu dat file not created!");
			}

		}

	};

}
