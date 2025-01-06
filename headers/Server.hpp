#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h>
#include <fstream>
#include <sstream>

#include "Socket.hpp"
#include "CompactList.tpp"
#include "FT.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "CGIHandler.hpp"
#include "Log.hpp"

#define RECV_SIZE 1024

class Server {

private:
	Server();
	Server(const Server& cpy);

	Server&	operator=(const Server& cpy);

public:
	Server(int port);

	void	start();

private:
	bool				running;
	Socket				socket;
	CompactList<pollfd>	fds;
	CGIHandler			cgi;

	bool	handle_event(pollfd& fd);
	void	handle_request(HTTP::Request& req, int fd);
};

#endif