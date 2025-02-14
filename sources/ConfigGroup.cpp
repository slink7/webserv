
#include "ConfigGroup.hpp"

ConfigGroup::ConfigGroup() :
	configs(std::vector<Config*>())
{}

ConfigGroup::~ConfigGroup()
{}

const Config &ConfigGroup::GetConfig(HTTP::Request &req) const {
	for (std::vector<Config*>::const_iterator it = configs.begin(); it != configs.end(); it++) {
		if (!req.GetHeaderValue("Host").compare((*it)->host))
			return (**it);
	}
	return (*configs.at(0));
}
