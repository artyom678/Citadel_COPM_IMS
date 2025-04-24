#pragma once

#include "config.hpp"
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <zlib.h>



class Processor {

	struct z_stream_wrapper {

		z_stream_wrapper(z_stream&, bool& is_init, Mode mode);

		z_stream_wrapper(const z_stream_wrapper&) = delete;
		z_stream_wrapper& operator=(const z_stream_wrapper&) = delete;

		z_stream& get_z_stream();

		bool& get_init();

		~z_stream_wrapper();

	private:
		z_stream& stream;
		Mode mode;
		bool& is_init;
	};


public:	

	Processor(Mode mode, const std::string& input_file, const std::string& output_file);

	bool process();

	static constexpr int CHUNK_SIZE = 128 * 1024;

private:

	bool compress();
	bool decompress();

	void remove_output_file(std::ofstream& output_file);
	void on_interrupt(std::ofstream& output_file);

	std::ofstream open_output();
	std::ifstream open_input();

private:

	Mode mode;
	std::string input_path;
	std::string output_path;

	std::vector<std::uint8_t> input_buffer;
	std::vector<std::uint8_t> output_buffer;

	z_stream stream;
	bool is_stream_init;

};


struct ProcessingError : public std::runtime_error {
	inline explicit ProcessingError(const std::string& error_msg) : std::runtime_error(error_msg) {}
};













