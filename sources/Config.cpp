#include "Config.hpp"



std::string Config::EvaluateRoute(const HTTP::Request &req) const {
	const std::string& path = req.GetTarget();
	
	if (path.size() == 0) {
		return (root + index);
	}
	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it++) {
		if (it->HasLimitedMethods() && !it->HasMethod(req.GetMethod()))
			continue ;
		
		std::size_t pos = path.find(it->path);
		if (pos != 0)
			continue ;

		std::string temp = it->root + path.substr(pos + it->path.size());
		
		if (FT::is_directory(temp)) {
			if (temp.at(temp.size() - 1) == '/')
				return (temp + it->index);
			else
				return (temp + '/' + index);
		} else
			return (temp);
	}
	if (FT::is_directory(path))
		return (root + path + index);
	else
		return (root + path);
}