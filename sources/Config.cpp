#include "Config.hpp"



std::string Config::EvaluateRoute(const HTTP::Request &req) const {
	const std::string& path = req.GetTarget();
	
	if (path.size() == 0) {
		return (root + index);
	}
	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it++) {
		Log::out(Log::DEBUG) << "Method: " << it->HasLimitedMethods() << ", " << it->HasMethod(req.GetMethod()) << "\n";
        
        if (it->HasLimitedMethods() && !it->HasMethod(req.GetMethod()))
			continue ;
		
        std::string t = it->path;
        FT::trim(t, " /");
		std::size_t pos = path.find(t);
		
        Log::out(Log::DEBUG) << "T: " << t << "\n";
        Log::out(Log::DEBUG) << "Pos: " << pos << "\n";
        
        if (pos != 0)
			continue ;

		std::string temp = it->root + path.substr(pos + t.size());
		
        Log::out(Log::DEBUG) << "Path: " << path << ", " << pos << ", " << t.size() << "\n";
        Log::out(Log::DEBUG) << "Root: " << it->root << ", " << path.substr(pos + t.size()) << "\n";
        Log::out(Log::DEBUG) << "Temp: " << temp << "\n";
        Log::out(Log::DEBUG) << "IsDir: " << FT::is_directory(temp) << "\n";

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