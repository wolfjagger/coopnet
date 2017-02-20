#include "plot_creator.h"
#include <algorithm>
#include <iterator>
#include <sstream>



namespace coopplot {

	PlotFile::PlotFile(
		string init_relative_location, string init_filename, const PlotStringFactory& factory) :
		relative_location(init_relative_location),
		filename(init_filename) {

		std::cout << factory.produce_file_str() << std::endl;

		out_file.open(full_path(), std::ios_base::out);
		out_file << factory.produce_file_str() << std::endl;
		out_file.close();

	}

	PlotFile::PlotFile(string init_relative_location, string init_filename) :
		PlotFile(init_relative_location, init_filename, TrivialStringFactory()) {

	}


	/*plotfile::plotfile(const plotfile& other, string new_location, string new_filename) :
		relative_location(new_location),
		filename(new_filename),
		out_file(full_path()) {

		auto line = string();

		if(std::ifstream is(other.full_path())) {
			
			out_file << line << std::endl;

		}

	}

	plotfile::plotfile(const plotfile& other, string new_location) :
		plotfile(other, new_location, other.filename) {

	}*/



	void PlotFile::mv(string new_relative_location, string new_filename) {

		auto new_plotfile = PlotFile(
			new_relative_location, new_filename, TrivialStringFactory());

		std::swap(new_plotfile.out_file, out_file);
		out_file.close();

		rm();

		std::swap(*this, new_plotfile);

	}

	void PlotFile::mv(string new_relative_location) {

		mv(new_relative_location, filename);

	}



	void PlotFile::rm() {

		out_file.close();

		auto path = full_path();
		auto remove_return = std::remove(path.c_str());

		if (remove_return == 0) {
			string err = "Cannot remove file ";
			err += path;
			throw std::exception(err.c_str());
		}

	}

	void PlotFile::change_data(const PlotStringFactory& factory) {

		rm();

		auto path = full_path();
		out_file = std::fstream(path);

		out_file << factory.produce_file_str() << std::endl;

		out_file.close();

	}



	std::string PlotFile::get_file_str() {

		out_file.open(full_path(), std::ios_base::in);

		auto stream = std::stringstream();

		using istreambuf_iter = std::istreambuf_iterator<char>;
		using ostreambuf_iter = std::ostreambuf_iterator<char>;
		std::copy(
			istreambuf_iter(out_file),
			istreambuf_iter(),
			ostreambuf_iter(stream));

		auto str = stream.str();
		std::cout << str;

		out_file.close();
		
		return str;
		
	}




	std::string PlotFile::full_path() const {

		return string(out_dir) + "/" + relative_location +  "/" + filename;

	}







	std::string TrivialStringFactory::produce_file_str() const {
		return "";
	}





	std::string GNUDatStringFactory::produce_file_str() const {

		return boost::apply_visitor(
			CreateDat<double, double>(),
			data.payload);

	}

	size_t GNUDatStringFactory::get_num_y_cols() const {
		return num_y_cols;
	}





	// GLOBAL OPTIONS

	void GNUScriptStringFactory::set_title(string title) {
		title_str = "set title ";
		title_str += gnu_quote + std::move(title) + gnu_quote;
	}



	// PER-PLOT OPTIONS

	void GNUScriptStringFactory::set_plot_titles(const std::vector<string>& plot_titles) {
		plot_title_strs = std::vector<string>();
		for (auto title : plot_titles) {
			auto str = std::string();
			str += "title ";
			str += gnu_quote + std::move(title) + gnu_quote;
			plot_title_strs.push_back(std::move(str));
		}
	}

	void GNUScriptStringFactory::set_x_range(RangeData<double> x_range) {
		x_range_str = "[" + std::to_string(x_range[0]);
		x_range_str += " to ";
		x_range_str += std::to_string(x_range[1]) + "]";
	}

	void GNUScriptStringFactory::set_y_range(RangeData<double> y_range) {
		y_range_str = "[" + std::to_string(y_range[0]);
		y_range_str += " to ";
		y_range_str += std::to_string(y_range[1]) + "]";
	}

	void GNUScriptStringFactory::set_options(std::string options) {
		options_str = std::move(options);
	}





	// Formats the string for calling gnuplot to plot the data.
	//  Dat file must be created before this call (throws otherwise).
	std::string GNUScriptStringFactory::produce_file_str() const {

		auto str = std::string();

		str += header;
		str += "\n";
		if (!title_str.empty()) {
			str += title_str;
			str += "\n";
		}
		str += plot_str();
		str += "\n";
		str += footer;

		return str;

	}



	std::string GNUScriptStringFactory::system_str(
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



	std::string GNUScriptStringFactory::plot_str() const {

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

	std::string GNUScriptStringFactory::core_str() const {

		std::string str;

		if (num_y_cols == 1) {

			str += gnu_quote + gnudat_path + gnu_quote;
			str += sep;
			if (plot_title_strs.size() >= 1) {
				str += plot_title_strs[0];
				str += sep;
			}
			str += options_str;
			str += sep;
			if (plot_title_strs.empty()) {
				str += "notitle";
				str += sep;
			}
			return str;

		} else {

			for (size_t i = 0; i < num_y_cols; ++i) {

				str += gnu_quote + gnudat_path + gnu_quote;
				str += sep;
				str += "using 1:" + std::to_string(i + 2);
				str += sep;
				if (plot_title_strs.size() >= i + 1) {
					str += plot_title_strs[i];
					str += sep;
				}
				str += options_str;
				if (i < num_y_cols - 1) {
					str += ",";
					str += sep;
				}
				
			}

			return str;

		}

	}

}
