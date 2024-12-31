#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h>
#include <fstream>
#include <sstream>

#include "Socket.hpp"
#include "CompactList.tpp"
#include "FT.hpp"
#include "Request.hpp"

#define RECV_SIZE 1024

class Server {

public:
	Server() = delete;
	Server(int port);
	Server(const Server& cpy) = delete;

	Server&	operator=(const Server& cpy) = delete;

	void	start();

private:
	bool				running;
	Socket				socket;
	CompactList<pollfd>	fds;
	// CGIHandler			cgi;

	bool	handle_event(pollfd& fd);
	void	handle_request(std::string& req, int fd);
};

#endif