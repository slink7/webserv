#include "Messenger.hpp"

int msg::send(int fd, std::string &msg, int len)
{
	return (send(fd, msg.c_str(), len));
}

int msg::send(int fd, const char *msg, int len)
{
	int	out;

	out = ::send(fd, msg, len, 0);
	if (out < 0) {
		std::cerr << "send() failed: " << strerror(errno) << "\n";
	}
	return (out);
}

int msg::send(int fd, std::string &msg)
{
	return (send(fd, msg.c_str(), msg.size()));
}

int msg::send(int fd, const char *msg)
{
	return (send(fd, msg, strlen(msg)));
}

int msg::receive(int fd, std::string &out)
{
	std::ostringstream stream;
	char	buffer[RECV_SIZE];
	int		len = 0;
	int		k;

	do {
		k = recv(fd, buffer, RECV_SIZE - 1, 0);
		if (k < 0) {
			std::cerr << "recv() failed: " << strerror(errno) << "\n";
			break ;
		}
		buffer[k] = 0;
		len += k;
		stream << buffer;
	} while (k >= RECV_SIZE - 1);
	out = stream.str();
	return (k);
}