#pragma once

#include <fstream>
#include "create_dat.h"
#include "xy_data.h"



namespace coopplot {

	class PlotStringFactory;

	class PlotFile {

	protected:

		using string = std::string;
		using fstream = std::fstream;
		using istream = std::istream;

		string out_dir = "../../out/";

		string relative_location;
		string filename;
		fstream out_file;

	public:

		PlotFile(string initrelative_location, string init_filename,
			const PlotStringFactory& factory);
		PlotFile(string init_relative_location, string initfilename);

		//TODO: Copy ctor for plotfile
		//plotfile(const plotfile& other, string new_location, string new_filename);
		//plotfile(const plotfile& other, string new_location);

		void mv(string new_relative_location, string new_filename);
		void mv(string new_relative_location);

		// Throws! And puts class into inconsistent state.
		void rm();

		void change_data(const PlotStringFactory& factory);

		string get_file_str();

		string full_path() const;

	};



	class PlotStringFactory {

	protected:

		using string = std::string;

		static constexpr char* sep = " ";

	public:

		virtual string produce_file_str() const = 0;

	};



	class TrivialStringFactory : public PlotStringFactory {

	public:

		string produce_file_str() const override {
			return "";
		}

	};



	template<typename XType, typename YType>
	class GNUDatStringFactory : public PlotStringFactory {

		XYData<XType, YType> data;

		size_t num_y_cols;

	public:

		GNUDatStringFactory(XYData<XType, YType> init_data) :
			data(init_data),
			num_y_cols(boost::apply_visitor(
				CalcNumYCols<XType, YType>(), data.payload)) {

		}

		string produce_file_str() const override {

			return boost::apply_visitor(
				CreateDat<XType, YType>(),
				data.payload);

		}

		size_t get_num_y_cols() const {
			return num_y_cols;
		}

	};



	template<typename XType, typename YType>
	class GNUScriptStringFactory : public PlotStringFactory {

	private:

		string header = "# gnuplot";
		string footer = "# end gnuplot";
		static constexpr char* gnu_quote = "\"";

		string title_str = "";

		std::vector<string> plot_title_strs = std::vector<string>();

		string x_range_str = "";
		string y_range_str = "";

		string options_str = "with lines";

		string gnudat_path;
		size_t num_y_cols;

	public:

		GNUScriptStringFactory() = default;

		GNUScriptStringFactory(string dat_path,
			RangeData<XType> x_range, RangeData<YType> y_range,
			size_t init_num_y_cols) :
			gnudat_path(dat_path),
			num_y_cols(init_num_y_cols) {

			set_x_range(x_range);
			set_y_range(y_range);
			
		}



		// GLOBAL OPTIONS

		// Set gnuplot title; defaults to ""
		void set_title(string title) {
			title_str = "set title ";
			title_str += gnu_quote + std::move(title) + gnu_quote;
		}



		// PER-PLOT OPTIONS

		// Set gnuplot plot titles; defaults to empty
		void set_plot_titles(const std::vector<string>& plot_titles) {
			plot_title_strs = std::vector<string>();
			for (auto title : plot_titles) {
				auto str = std::string();
				str += "title ";
				str += gnu_quote + std::move(title) + gnu_quote;
				plot_title_strs.push_back(std::move(str));
			}
		}

		// Set min and max x; default chosen by gnuplot
		void set_x_range(RangeData<XType> x_range) {
			x_range_str = "[" + std::to_string(x_range[0]);
			x_range_str += " to ";
			x_range_str += std::to_string(x_range[1]) + "]";
		}

		// Set min and max y; default chosen by gnuplot
		void set_y_range(RangeData<YType> y_range) {
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
			str += "\n";
			if(!title_str.empty()) {
				str += title_str;
				str += "\n";
			}
			str += plot_str();
			str += "\n";
			str += footer;

			return str;

		}



		static std::string system_str(
			std::string script_string) {
			auto str = std::string();
			str += "gnuplot -p -e ";
			str += "\"";
			str += "load ";
			str += "\\\"";
			str += std::move(script_string);
			str += "\\\"";
			str += "\"";
			return str;
		}



	private:

		std::string plot_str() const {

			std::string str;
			str += "plot ";
			str += sep;
			str += x_range_str;
			str += sep;
			str += y_range_str;
			str += sep;
			str += core_str();
			return str;

		}

		std::string core_str() const {

			std::string str;

			if(num_y_cols == 1) {

				str += gnu_quote + gnudat_path + gnu_quote;
				str += sep;
				if(plot_title_strs.size() >= 1) {
					str += plot_title_strs[0];
					str += sep;
				}
				str += options_str;
				str += sep;
				if(plot_title_strs.empty()) {
					str += "notitle";
					str += sep;
				}
				return str;

			} else {

				for(size_t i=0; i<num_y_cols; ++i) {

					str += gnu_quote + gnudat_path + gnu_quote;
					str += sep;
					str += "using 1:" + std::to_string(i+2);
					str += sep;
					if (plot_title_strs.size() >= i + 1) {
						str += plot_title_strs[i];
						str += sep;
					}
					str += options_str;
					if(i < num_y_cols - 1) {
						str += ",";
						str += sep;
					}

				}

				return str;

			}

		}

	};

}
