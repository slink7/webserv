#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cerrno>

#include "Log.hpp"

class Socket {

private:
	Socket();
	Socket(const Socket &cpy);

	Socket&	operator=(const Socket &cpy);

public:
	Socket(unsigned short port, int queue);
	~Socket();

	int			get_fd() const;
	bool		is_enabled() const;
	int			set_flag(int flag, bool enabled);
	static int	set_flag(int fd, int flag, bool enabled);

private:
	bool	enabled;
	int		fd;
};

#endif