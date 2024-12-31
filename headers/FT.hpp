#ifndef FT_HPP
#define FT_HPP

#include <string>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace FT {
	const int RECV_SIZE = 17;

	int	send(int fd, std::string& msg, int len);
	int	send(int fd, const char *msg, int len);
	int	send(int fd, std::string& msg);
	int	send(int fd, const char *msg);

	int	receive(int fd, std::string& out);

	int	read(int fd, std::string& out);
}

#endif