#include "Socket.hpp"

Socket::Socket(unsigned short port, int queue) :
	enabled(false)
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		std::cerr << "socket() failed: " << strerror(errno) << "\n";
		return ;
	}

	int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "setsockopt() failed: " << strerror(errno) << "\n";
		return ;
	}

	struct sockaddr_in	addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = 0;

	if (bind(fd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(fd);
		std::cerr << "bind() failed: " << strerror(errno) << "\n";
		return ;
	}

	if (listen(fd, queue) < 0) {
		close(fd);
		std::cerr << "listen() failed: " << strerror(errno) << "\n";
		return ;
	}

	enabled = true;
}

Socket::~Socket() {
	if (enabled && fd >= 0)
		close(fd);
}

int Socket::get_fd() const {
	return (fd);
}

bool Socket::is_enabled() const
{
	return (enabled);
}
