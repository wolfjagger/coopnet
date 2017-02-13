#pragma once

#include <fstream>
#include "create_dat.h"
#include "xy_data.h"



namespace coopplot {

	template<typename x_type, typename y_type>
	class plot_creator {

	private:

		static constexpr char* out_dir = "../../out/";

		static constexpr char* header = "gnuplot -p -e \"plot";
		static constexpr char* sep = " ";
		static constexpr char* footer = "\"";
		static constexpr char* gnu_quote = "\\\"";

		std::string x_range_str = "";
		std::string y_range_str = "";
		std::string options_str = "with lines";

		std::unique_ptr<std::string> p_filename;
		size_t num_y_cols;

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
			std::string filename,
			xy_data<x_type, y_type> sat_data) {

			// Prepend the output directory
			filename = std::string(out_dir) + filename;

			p_filename
				= std::make_unique<std::string>(std::move(filename));

			// Generate dat file string
			auto file_str = boost::apply_visitor(
				create_dat<double, double>(),
				sat_data.payload);

			// Create dat file from string
			std::ofstream file(*p_filename);
			file << std::move(file_str) << std::endl;

			num_y_cols = boost::apply_visitor(
				calc_num_y_cols<double, double>(),
				sat_data.payload);

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
				str += core_str();
				str += footer;

				return str;

			} else {
				throw std::exception(
					"gnu dat file not created!");
			}

		}

		std::string core_str() const {

			if(num_y_cols == 1) {

				std::string str;
				str += gnu_quote + *p_filename + gnu_quote;
				str += sep;
				str += options_str;
				str += sep;
				return str;

			} else {

				std::string str;
				for(auto i=0; i<num_y_cols; ++i) {

					str += gnu_quote + *p_filename + gnu_quote;
					str += sep;
					str += "using 1:" + std::to_string(i+2);
					str += sep;
					str += options_str;
					str += ",";
					str += sep;

				}

				return str;

			}

		}

	};

}
