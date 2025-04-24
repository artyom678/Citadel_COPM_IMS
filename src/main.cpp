#include <exception>
#include <iostream>
#include <exception>
#include "signal_handler.hpp"
#include "config.hpp"
#include "processor.hpp"


int main(int argc, char* argv[]) {


	SignalHandler::setup_sigint_handler();

	try {
		
		AppConfig config(argc, argv);

		Processor proc(config.get_mode(), config.get_input_file(), config.get_output_file());

		proc.process(); // process() returns the result (success or failure), but we don't need it here

		if (SignalHandler::isInterrupted()) {
			std::cerr << "User interrupted the program by SIGINT signal...\n";
			return 1;
		}

		return 0;

	}
	catch(ConfigError& err) {
		std::cerr << "Configuration error : " << err.what() << std::endl;
		return 1;
	}
	catch(ProcessingError& err) {
		std::cerr << "Processing error : " << err.what() << std::endl;
		return 2;
	}
	catch(std::exception& err) {
		std::cerr << "Another error : " << err.what() << std::endl;
		return 3;
	}
	catch(...) {
		std::cerr << "Unknown error" << std::endl;
		return 4;
	}

}

//Problems:
//1) RAII-wrapper for z_stream
//2) Deletion of the output file in case of error