#ifndef MESSENGER_HPP
#define MESSENGER_HPP

#include <string>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>

namespace msg {
	const int RECV_SIZE = 17;

	int	send(int fd, std::string& msg, int len);
	int	send(int fd, const char *msg, int len);
	int	send(int fd, std::string& msg);
	int	send(int fd, const char *msg);

	int	receive(int fd, std::string& out);
}

#endif