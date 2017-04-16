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

		string produce_file_str() const override;

	};



	class GNUDatStringFactory : public PlotStringFactory {

		XYData<double, double> data;

		size_t num_y_cols;

	public:

		template<typename XType, typename YType>
		GNUDatStringFactory(XYData<XType, YType> init_data) :
			data(init_data),
			num_y_cols(boost::apply_visitor(
				CalcNumYCols<XType, YType>(), data.payload)) {

		}

		string produce_file_str() const override;

		size_t get_num_y_cols() const;

	};



	class GNUScriptStringFactory : public PlotStringFactory {

	private:

		string header = "# gnuplot";
		string footer = "# end gnuplot";
		static constexpr char* gnu_quote = "\"";

		string title_str = "";

		std::vector<string> plot_title_strs = std::vector<string>();

		string x_range_str = "";
		string y_range_str = "";

		string options_str;

		string gnudat_path;
		size_t num_y_cols;

		bool withError;

	public:

		GNUScriptStringFactory() = default;

		template<typename XType, typename YType>
		GNUScriptStringFactory(string dat_path,
			RangeData<XType> x_range, RangeData<YType> y_range,
			size_t init_num_y_cols, bool initWithError) :
			gnudat_path(dat_path),
			num_y_cols(init_num_y_cols),
			withError(initWithError) {

			set_x_range(x_range);
			set_y_range(y_range);

			if (withError) {
				options_str = "with errorlines";
			} else {
				options_str = "with lines";
			}
			
		}



		// GLOBAL OPTIONS

		// Set gnuplot title; defaults to ""
		void set_title(string title);



		// PER-PLOT OPTIONS

		// Set gnuplot plot titles; defaults to empty
		void set_plot_titles(const std::vector<string>& plot_titles);

		// Set min and max x; default chosen by gnuplot
		void set_x_range(RangeData<double> x_range);

		// Set min and max y; default chosen by gnuplot
		void set_y_range(RangeData<double> y_range);

		// Add options; defaults to "with lines" without error
		//  or "with errorlines" with error
		void set_options(std::string options);



		

		// Formats the string for calling gnuplot to plot the data.
		//  Dat file must be created before this call (throws otherwise).
		string produce_file_str() const override;



		static std::string system_str(std::string script_string);



	private:

		string plot_str() const;

		string core_str() const;

	};

}
