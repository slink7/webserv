#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

class Socket {
public:
	Socket() = delete;
	Socket(unsigned short port, int queue);
	~Socket();

	int		get_fd() const;
	bool	is_enabled() const;

private:
	bool	enabled;
	int		fd;
};

#endif