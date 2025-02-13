#include "Config.hpp"

int Config::EvaluateRoute(std::string& out, const HTTP::Request &req) const {
	int wrong_method_count = 0;
    const std::string& path = req.GetTarget();
    Log::out(Log::REQUEST) << path << "\n";
	
	if (path.size() == 0) {
		out = root + index;
		return (200);
	}
	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it++) {
		
        std::string t = it->path;
        FT::trim(t, " /");
		std::size_t pos = path.find(t);
        
        if (pos != 0)
			continue ;
        
        if (it->HasLimitedMethods() && !it->HasMethod(req.GetMethod())) {
            wrong_method_count++;
			continue ;
        }

		std::string temp = it->root + path.substr(t.size());

		if (FT::is_directory(temp)) {
			if (temp.at(temp.size() - 1) == '/')
				out = temp + it->index;
			else
				out = temp + '/' + index;
		} else
    		out = temp;
		return (200);
	}
    
    if (wrong_method_count)
        return (405);

    std::string temp = root + path;
	if (FT::is_directory(temp))
		out = temp + "/" + index;
	else
		out = temp;
	return (200);
}