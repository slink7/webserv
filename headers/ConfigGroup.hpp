#ifndef CONFIGGROUP_HPP
#define CONFIGGROUP_HPP

#include <vector>

#include "Config.hpp"

class ConfigGroup {
	std::vector<Config>			configs;
	std::vector<unsigned short>	port;
};

#endif