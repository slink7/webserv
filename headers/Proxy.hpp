#ifndef PROXY_HPP
#define PROXY_HPP

#include <poll.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <map>
#include <algorithm>

#include "Request.hpp"
#include "Response.hpp"
#include "Log.hpp"
#include "FT.hpp"
#include "ConfigGroup.hpp"

class Proxy {

public:
	Proxy();

	bool							AddGroup(ConfigGroup& config);
	bool							AddSocket(ConfigGroup& config, unsigned short port);
	void							AddFD(int fd, int events);
	void							AddSocketsToPoll();
	std::vector<pollfd>::iterator	RemoveClient(std::vector<pollfd>::iterator it);
	void							CloseFDs();
	bool							HandleEvent(std::vector<pollfd>::iterator& it);
	void							Run();
	void							Clear();

private:
	static const int			max_fds_count;

	std::map<int, ConfigGroup*>	configs;
	std::vector<int>			sockets;
	std::vector<pollfd>			fds;
	std::map<int, int>			parents;
	std::map<int, std::string>	requests;

};

#endif