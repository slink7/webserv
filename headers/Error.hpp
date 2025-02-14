#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error {
public:
	int			code;
	std::string	status;
	std::string	default_path;
	std::string	custom_path;

	Error();
	Error(int code, const std::string& status, const std::string& default_path, const std::string& custom_path);
	private:
};

#endif