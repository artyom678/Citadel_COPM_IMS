#pragma once

#include <string>
#include <stdexcept>

//correct this (I suppose, that enum would be enough...)
enum class Mode {
	COMPRESS,
	DECOMPRESS,
	UNKNOWN
};


struct AppConfig {

	AppConfig(int argc, char* argv[]);

	Mode get_mode() const noexcept;

	const std::string& get_input_file() const noexcept;

	const std::string& get_output_file() const noexcept;

private:

	void parse_arguments(int argc, char* argv[]);

private:

	Mode mode = Mode::UNKNOWN;
	std::string input_file;
	std::string output_file;
};


struct ConfigError : public std::runtime_error {
	explicit ConfigError(const std::string& error_msg) : std::runtime_error(error_msg) {}
};