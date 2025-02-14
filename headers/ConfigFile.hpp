#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include "CGI.hpp"

#include "Method.hpp"
#include "ConfigGroup.hpp"
#include "Config.hpp"

class ConfigFile
{

private:
public:
	std::map<unsigned short, ConfigGroup> port_config;
	std::vector<Config> config_list;
	std::string _content;
	
	ConfigFile(const std::string& path);
	~ConfigFile();
	void startconfig(std::string _content);
	void	LoadFromFile(const std::string& path);
};

#endif