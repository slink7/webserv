#include "Error.hpp"

Error::Error(int code, const std::string &status, const std::string &default_path, const std::string &custom_path) :
	code(code),
	status(status),
	default_path(default_path),
	custom_path(custom_path)
{}

Error::Error() :
	code(-1),
	status(""),
	default_path(""),
	custom_path("")
{}