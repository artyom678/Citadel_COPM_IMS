#include "config.hpp"
#include <iostream>


AppConfig::AppConfig(int argc, char* argv[]) {
	parse_arguments(argc, argv);	
}


void AppConfig::parse_arguments(int argc, char* argv[]) {
	
	if (argc != 4) {
		std::cerr << "Usage : " << argv[0] << " -c|-d <input file> <output_file>\n";
		std::cerr << " -c : compress <input_file> to <output_file>\n";
		std::cerr << " -d : decompress <input_file> to <output_file>\n";
		throw ConfigError("Invalid number of arguments!"); //// 
	}

	std::string mode_flag = argv[1];

	if (mode_flag == "-c") {
		mode = Mode::COMPRESS;
	}
	else if (mode_flag == "-d") {
		mode = Mode::DECOMPRESS;
	}
	else {
		throw ConfigError("Invalid mode flag: use -c or -d");
	}

	input_file = argv[2];
	output_file = argv[3];

	if (input_file.empty() || output_file.empty()) {
		throw ConfigError("Input and output filenames can't be empty!"); //
	}
	else if (input_file == output_file) {
		throw ConfigError("Input and output files can't be the same!");
	}
}


Mode AppConfig::get_mode() const noexcept {
	return mode;
}

const std::string& AppConfig::get_input_file() const noexcept {
	return input_file;
}

const std::string& AppConfig::get_output_file() const noexcept {
	return output_file;
}

