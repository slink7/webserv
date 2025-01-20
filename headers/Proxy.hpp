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

struct Config {
	unsigned short port;
};

class Proxy {

public:
	Proxy();

	bool							AddServer(const Config& config);
	void							AddFD(int fd, int events);
	void							AddSocketsToPoll();
	std::vector<pollfd>::iterator	RemoveClient(std::vector<pollfd>::iterator it);
	void							CloseFDs();
	bool							HandleEvent(std::vector<pollfd>::iterator& it);
	void							Run();

private:
	static const int				max_fds_count;

	std::map<int, Config>			configs;
	std::vector<int>				sockets;
	std::vector<pollfd>				fds;
	std::map<int, int>				parents;
	std::map<int, HTTP::Request>	requests;

};

#endif