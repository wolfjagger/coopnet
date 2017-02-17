#pragma once

#include <fstream>
#include "create_dat.h"
#include "xy_data.h"



namespace coopplot {

	class plot_string_factory;

	class plotfile {

	protected:

		using string = std::string;
		using fstream = std::fstream;
		using istream = std::istream;

		string out_dir = "../../out/";

		string relative_location;
		string filename;
		fstream out_file;

	public:

		plotfile(string initrelative_location, string init_filename,
			const plot_string_factory& factory);
		plotfile(string init_relative_location, string initfilename);

		//TODO: Copy ctor for plotfile
		//plotfile(const plotfile& other, string new_location, string new_filename);
		//plotfile(const plotfile& other, string new_location);

		//TODO: Move methods for plotfile
		void mv(string new_relative_location, string new_filename);
		void mv(string new_relative_location);

		// Throws! And puts class into inconsistent state.
		void rm();

		void change_data(const plot_string_factory& factory);

		string get_file_str();

		string full_path() const;

	};



	class plot_string_factory {

	protected:

		using string = std::string;

		static constexpr char* sep = " ";

	public:

		virtual string produce_file_str() const = 0;

	};



	class trivial_string_factory : public plot_string_factory {

	public:

		string produce_file_str() const override {
			return "";
		}

	};



	template<typename x_type, typename y_type>
	class gnudat_string_factory : public plot_string_factory {

		xy_data<x_type, y_type> data;

		size_t num_y_cols;

	public:

		gnudat_string_factory(xy_data<x_type, y_type> init_data) :
			data(init_data),
			num_y_cols(boost::apply_visitor(
				calc_num_y_cols<x_type, y_type>(), data.payload)) {

		}

		string produce_file_str() const override {

			return boost::apply_visitor(
				create_dat<x_type, y_type>(),
				data.payload);

		}

		size_t get_num_y_cols() const {
			return num_y_cols;
		}

	};



	template<typename x_type, typename y_type>
	class gnuscript_string_factory : public plot_string_factory {

	private:

		string x_range_str = "";
		string y_range_str = "";

		string header = "gnuplot -p -e \"plot";
		string footer = "\"";
		static constexpr char* gnu_quote = "\\\"";

		string options_str = "with lines";
		string gnudat_path;

		size_t num_y_cols;

	public:

		gnuscript_string_factory() = default;

		gnuscript_string_factory(string dat_path,
			range_data<x_type> x_range, range_data<y_type> y_range,
			size_t init_num_y_cols) :
			gnudat_path(dat_path),
			num_y_cols(init_num_y_cols) {

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



		

		// Formats the string for calling gnuplot to plot the data.
		//  Dat file must be created before this call (throws otherwise).
		string produce_file_str() const override {

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

		}



	private:

		std::string core_str() const {

			if(num_y_cols == 1) {

				std::string str;
				str += gnu_quote + gnudat_path + gnu_quote;
				str += sep;
				str += options_str;
				str += sep;
				return str;

			} else {

				std::string str;
				for(size_t i=0; i<num_y_cols; ++i) {

					str += gnu_quote + gnudat_path + gnu_quote;
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
