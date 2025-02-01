#ifndef CONFIGGROUP_HPP
#define CONFIGGROUP_HPP

#include <vector>

#include "Config.hpp"
#include "Request.hpp"

class ConfigGroup {
public:
	std::vector<Config>			configs;
	std::vector<unsigned short>	port;

	ConfigGroup();

	const Config&	GetConfig(HTTP::Request& req) const;
};

#endif