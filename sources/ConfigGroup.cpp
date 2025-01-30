
#include "ConfigGroup.hpp"

const Config &ConfigGroup::GetConfig(HTTP::Request &req) const {
	for (std::vector<Config>::const_iterator it = configs.begin(); it != configs.end(); it++) {
		if (!req.GetHeaderValue("Host").compare(it->host))
			return (*it);
	}
	return (configs.at(0));
}