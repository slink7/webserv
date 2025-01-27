#ifndef CGI_HPP
#define CGI_HPP

#include <map>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <sys/wait.h>
#include <cerrno>
#include <stdlib.h>

#include "Response.hpp"
#include "Request.hpp"
#include "FT.hpp"

namespace HTTP {
	class CGI {

	public:
		bool	Handle(const HTTP::Request& req, int fd) const;
		void	Add(const std::string& extension, const std::string& executable_path);

	private:
		std::map<std::string, std::string> cgis;

		std::map<std::string, std::string>::const_iterator GetIterator(const std::string& path) const;
	};
}

#endif