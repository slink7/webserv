#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

class Socket {
public:
	Socket() = delete;
	Socket(const Socket &cpy) = delete;
	Socket(unsigned short port, int queue);
	~Socket();

	Socket&	operator=(const Socket &cpy) = delete;

	int			get_fd() const;
	bool		is_enabled() const;
	int			set_flag(int flag, bool enabled);
	static int	set_flag(int fd, int flag, bool enabled);

private:
	bool	enabled;
	int		fd;
};

#endif