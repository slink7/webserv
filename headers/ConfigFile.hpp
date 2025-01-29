#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <string>
#include <map>
#include <vector>

#include "CGI.hpp"
#include "Method.hpp"
#include "ConfigGroup.hpp"

class ConfigFile {

public:

	ConfigFile(const std::string& path);
	
	bool	LoadFromFile(const std::string& path);

private:

	std::vector<ConfigGroup> groups;
};

#endif