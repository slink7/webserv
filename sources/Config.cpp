#include "Config.hpp"



std::string Config::EvaluateRoute(const std::string &path) const {
	if (path.size() == 0) {
		return (root + index);
	}
	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it++) {
		std::string p;
		if (it->path.at(it->path.size() - 1) == '/')
			p = it->path.substr(0, it->path.size() - 1);
		else
			p = it->path;

		
		std::size_t pos = path.find(p);
		Log::out(Log::DEBUG) << "In: " << path << ", found " << p << ", at " << pos << "\n";
		if (pos != 0)
			continue ;
		std::string temp = it->root + path.substr(pos + p.size());
		Log::out(Log::DEBUG) << "Temp: " << temp << ".\n";
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