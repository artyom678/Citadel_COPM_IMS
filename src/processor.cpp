#include "processor.hpp"
#include "config.hpp"
#include "signal_handler.hpp"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <zlib.h>


Processor::Processor(Mode mode, const std::string& input_path, const std::string& output_path) 
	: mode(mode)
	, input_path(input_path)
	, output_path(output_path)
	, input_buffer(CHUNK_SIZE)
	, output_buffer(CHUNK_SIZE)
	, is_stream_init(false)
	, stream({})
{}



bool Processor::process() {

	is_stream_init = false; //

	if (mode == Mode::COMPRESS) {
		std::cout << "Compressing " + input_path + " to " + output_path << "...\n";
		return compress(); 
	}
	else if (mode == Mode::DECOMPRESS) {
		std::cout << "Decompressing " + input_path + " to " + output_path << "...\n";
		return decompress();
	}
	else {
		throw ProcessingError("Invalid mode specified for Processor!");
	}

}


bool Processor::compress() {

	std::ifstream source = open_input();
	std::ofstream dest = open_output();

	z_stream_wrapper wrapper(stream, is_stream_init, Mode::COMPRESS); //calls deflateInit in constructor and deflateEnd in destructor

	int ret;
	int flush;
	
	try {
		do {

			if (SignalHandler::isInterrupted()) {
				on_interrupt(dest);
				return false;
			}

			source.read(reinterpret_cast<char*>(input_buffer.data()), CHUNK_SIZE);
			stream.avail_in = static_cast<unsigned>(source.gcount());

			if (source.bad()) {
				throw ProcessingError("Error on reading the input file");
			}

			stream.next_in = input_buffer.data();
			flush = source.eof() ? Z_FINISH : Z_NO_FLUSH;
			

			do {

				if (SignalHandler::isInterrupted()) {
					on_interrupt(dest);
					return false;
				}

				stream.avail_out = CHUNK_SIZE;
				stream.next_out = output_buffer.data();
				ret = deflate(&stream, flush);

				if (ret == Z_STREAM_ERROR) {
					throw ProcessingError("Error on deflate (Z_STREAM_ERROR)");
				}			

				unsigned bytes_used = CHUNK_SIZE - stream.avail_out;
				if (!dest.write(reinterpret_cast<char*>(output_buffer.data()), bytes_used)) {
					throw ProcessingError("Failed to write to the output file");
				}
			}
			while(stream.avail_out == 0);

			if (stream.avail_in != 0) {
				throw ProcessingError("zlib deflate() failed to compress the whole chunk");
			}

		}
		while(flush != Z_FINISH);


		if (ret != Z_STREAM_END) {
			throw ProcessingError("deflate did't finish processing the stream correctly");
		}
	}
	catch(...) {
		remove_output_file(dest);
		throw;
	} 

	std::cout << "Compression finished successfully!\n";
	return true;
}




bool Processor::decompress() {

	std::ifstream source = open_input();
	std::ofstream dest = open_output();

	z_stream_wrapper wrapper(stream, is_stream_init, Mode::DECOMPRESS); // calls inflateInit in constructor and inflateEnd in destructor
	int ret;

	try {
		do {

			if (SignalHandler::isInterrupted()) {
				on_interrupt(dest);
				return false;
			}

			source.read(reinterpret_cast<char*>(input_buffer.data()), CHUNK_SIZE);
			stream.avail_in = source.gcount();

			if (source.bad()) { 
				throw ProcessingError("Error on reading the input file");
			}

			if (stream.avail_in == 0) {
				throw ProcessingError("Given input file" + input_path + " is invalid");
			}

			stream.next_in = input_buffer.data();

			do {

				if (SignalHandler::isInterrupted()) {
					on_interrupt(dest);
					return false;
				}

				stream.avail_out = CHUNK_SIZE;
				stream.next_out = output_buffer.data();

				ret = inflate(&stream, Z_NO_FLUSH);

				if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR || ret == Z_STREAM_ERROR) {

					std::string error_msg = stream.msg ? stream.msg : "no details";
					throw ProcessingError("zlib inflate error: " + error_msg);
				}	

				unsigned bytes_used = CHUNK_SIZE - stream.avail_out; // maybe we should somehow change CHUNK_SIZE (make it static)
				if (!dest.write(reinterpret_cast<char*>(output_buffer.data()), bytes_used)) {
					throw ProcessingError("Failed to write to the output file");
				}


			}
			while(stream.avail_out == 0);

			if (stream.avail_in != 0) {
				throw ProcessingError("zlib inflate() failed to decompress the whole chunk");
			}

		}
		while(ret != Z_STREAM_END);
	}
	catch(...) {
		remove_output_file(dest);
		throw;
	}

	std::cout << "Decompression finished successfully!\n";
	return true;	
}


std::ofstream Processor::open_output() {
	
	std::ofstream dest(output_path, std::ios::binary | std::ios::trunc);
	if (!dest) {
		throw ProcessingError("Failed to open the output file " + output_path);
	}

	return dest;
}



std::ifstream Processor::open_input() {

	std::ifstream source(input_path, std::ios::binary);
	if (!source) {
		throw ProcessingError("Failed to open the input file " + input_path);
	}

	return source;
}


void Processor::remove_output_file(std::ofstream& output_file) {

	if (output_file.is_open()) {
		output_file.close();
	}

	if (std::remove(output_path.c_str()) != 0) {
		std::cerr << "Failed to remove the output file " + output_path  << " : " << strerror(errno) << std::endl;
	}
}


void Processor::on_interrupt(std::ofstream& output_file) {

	std::cerr << "\nInterrupted by user. Cleaning up and removing incomplete output file...\n";
	if (!is_stream_init) {
		return;
	}

	remove_output_file(output_file);
}



Processor::z_stream_wrapper::z_stream_wrapper(z_stream& stream, bool& is_init, Mode mode) 
	: stream(stream)
	, mode(mode)
	, is_init(is_init)
{
	int ret;

	if (mode == Mode::COMPRESS) {
		ret = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
		if (ret != Z_OK) {
			throw ProcessingError("zlib deflateInit2 failed");
		}
	}
	else {
		ret = inflateInit2(&stream, 15 + 16); // this step is not really clear
		if (ret != Z_OK) {
			throw ProcessingError("An error occured on zlib inflateInit2");
		}
	}

	is_init = true;
}

Processor::z_stream_wrapper::~z_stream_wrapper() {

	if (mode == Mode::COMPRESS) {
		deflateEnd(&stream);
	}
	else {
		inflateEnd(&stream);
	}

	is_init = false;
}

z_stream& Processor::z_stream_wrapper::get_z_stream() {
	return stream;
}

bool& Processor::z_stream_wrapper::get_init() {
	return init;
}


