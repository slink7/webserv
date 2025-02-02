#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include <vector>

#include "Location.hpp"
#include "CGI.hpp"

struct Config {
	std::string					host;
	std::string					server_name;
	std::string					root;
	std::string					index;
	unsigned long				max_body_length;
	std::map<int, std::string>	error_pages;
	std::vector<Location>		locations;
	HTTP::CGI					cgi;

	std::string	EvaluateRoute(const std::string& path) const;
	std::string EvaluateTargetFile(const std::string& path) const;
};

#endif