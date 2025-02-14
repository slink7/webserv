#ifndef CONFIGGROUP_HPP
#define CONFIGGROUP_HPP

#include <vector>
#include <iostream>
#include <cstdlib>

#include "Config.hpp"

class ConfigGroup;

class ConfigGroup
{
	public :
		std::vector<Config*>			configs;
		ConfigGroup();
		~ConfigGroup();
		void print();
		const Config &GetConfig(HTTP::Request &req) const;
};

#endif