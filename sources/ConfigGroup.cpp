
#include "ConfigGroup.hpp"

ConfigGroup::ConfigGroup() :
	configs(std::vector<Config>()),
	port(std::vector<unsigned short>())
{
	std::cout << "New Group\n";
}

const Config &ConfigGroup::GetConfig(HTTP::Request &req) const
{
	std::cout << "Config size : " << configs.size() << std::endl;
	for (std::vector<Config>::const_iterator it = configs.begin(); it != configs.end(); it++) {
		if (!req.GetHeaderValue("Host").compare(it->host))
			return (*it);
	}
	return (configs.at(0));
}