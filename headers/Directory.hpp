#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <string.h>
#include <algorithm>
#include <sys/socket.h>

#include "FT.hpp"
#include "Log.hpp"

class ServerConfig;

class Directory {

public:
	void	AddSocket();
	void	AcceptNew();
	void	Close();

	void	AddClient(int fd, int socket);

	void	HandleEvents();

	bool	IsSocket(int fd);

private:
	std::vector<int>	sockets;
	std::vector<pollfd>	fd_events;
	// fd client -> fd socket parent
	std::map<int, int>	parents;
	// fd socket parent -> config associée
	std::map<int, ServerConfig> configs;
};

#endif