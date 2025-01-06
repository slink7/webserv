#include "Socket.hpp"

Socket::Socket(unsigned short port, int queue) :
	enabled(false)
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		Log::out(Log::FUNCTION) << "socket() failed: " << strerror(errno) << "\n";
		return ;
	}

	int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        Log::out(Log::FUNCTION) << "setsockopt() failed: " << strerror(errno) << "\n";
		return ;
	}

	struct sockaddr_in	addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = 0;

	if (bind(fd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(fd);
		Log::out(Log::FUNCTION) << "bind() failed: " << strerror(errno) << "\n";
		return ;
	}

	if (listen(fd, queue) < 0) {
		close(fd);
		Log::out(Log::FUNCTION) << "listen() failed: " << strerror(errno) << "\n";
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

int Socket::set_flag(int flag, bool enabled) {
	return (set_flag(fd, flag, enabled));
}

int Socket::set_flag(int fd, int flag, bool enabled)
{
	int flags = fcntl(fd, F_GETFL);
	return (fcntl(fd, F_SETFL, enabled ? (flags | flag) : (flags & ~flag)));
}
