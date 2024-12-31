#ifndef CGIHandler_HPP
#define CGIHandler_HPP

#include <map>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <cerrno>

#include "FT.hpp"

class CGIHandler {

public:
	bool	handle(const std::string& path) const;
	void	add(const std::string& extension, const std::string& executable_path);

private:
	std::map<std::string, std::string> cgis;

	std::map<std::string, std::string>::const_iterator get_iterator(const std::string& path) const;
};

#endif