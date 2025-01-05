#ifndef FT_HPP
#define FT_HPP

#include <string>
#include <cstring>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cerrno>
#include <sys/stat.h>

namespace FT {
	const int RECV_SIZE = 17;

	int	send(int fd, std::string& msg, int len);
	int	send(int fd, const char *msg, int len);
	int	send(int fd, std::string& msg);
	int	send(int fd, const char *msg);

	int	receive(int fd, std::string& out);

	int	read(int fd, std::string& out);

	std::string itoa(int n);

	bool	is_directory(const std::string& path);
	bool	is_file(const std::string& path);

	const std::string& get_mime_type(const std::string& file);

	void	replace(std::string& str, const char *from, const char *to);
}

#endif