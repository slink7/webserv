#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <string>
#include <map>
#include <vector>


#include "CGI.hpp"
#include "Method.hpp"

// #include "Config.hpp"

struct Location {
    std::string					path;
    std::string					root;
    std::string					index;
    std::vector<HTTP::Method>	methods;
};

class Config {
	std::string					host;
	std::string					server_name;
	std::string					root;
	std::string					index;
	unsigned long				max_body_length;
	std::map<int, std::string>	error_pages;
	std::vector<Location>		locations;
	HTTP::CGI					cgi;
};

class ConfigGroup {
	std::vector<Config> configs;
	unsigned short		port;
}

class ConfigFile {

public:

	ConfigFile(const std::string& path);
	
	bool	LoadFromFile(const std::string& path);

private:

	std::vector<ConfigGroup> groups;
};

#endif