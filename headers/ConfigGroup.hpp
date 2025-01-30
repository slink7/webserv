#ifndef CONFIGGROUP_HPP
#define CONFIGGROUP_HPP

#include <vector>

#include "Config.hpp"
#include "Request.hpp"

struct ConfigGroup {
	std::vector<Config>			configs;
	std::vector<unsigned short>	port;

	const Config&	GetConfig(HTTP::Request& req) const;
};

#endif