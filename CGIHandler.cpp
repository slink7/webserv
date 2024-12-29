#include "CGIHandler.hpp"

bool	CGIHandler::handle(const std::string& path) const {
	std::map<std::string, std::string>::const_iterator it = get_iterator(path);
	if (it == cgis.end())
		return (false);
	std::cout << "Handling " << path << "\n";
	std::cout << "Executing " << it->second << "\n";
	return (true);
}

void	CGIHandler::add(const std::string& extension, const std::string& executable_path) {
	cgis.insert({extension, executable_path});
}

std::map<std::string, std::string>::const_iterator	CGIHandler::get_iterator(const std::string& path) const {
	int	index = path.find_last_of('.');
	if (index == std::string::npos)
		return (cgis.end());
	std::string ext = path.substr(index);
	std::map<std::string, std::string>::const_iterator it = cgis.find(ext);
	return (it);
}