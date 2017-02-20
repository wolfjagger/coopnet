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

}
