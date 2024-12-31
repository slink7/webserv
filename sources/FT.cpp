#include "FT.hpp"

int FT::send(int fd, std::string &msg, int len) {
	return (send(fd, msg.c_str(), len));
}

int FT::send(int fd, const char *msg, int len) {
	int	out;

	out = ::send(fd, msg, len, 0);
	if (out < 0) {
		std::cerr << "send() failed: " << strerror(errno) << "\n";
	}
	return (out);
}

int FT::send(int fd, std::string &msg) {
	return (send(fd, msg.c_str(), msg.size()));
}

int FT::send(int fd, const char *msg) {
	return (send(fd, msg, strlen(msg)));
}

int FT::receive(int fd, std::string &out) {
	std::ostringstream stream;
	char	buffer[RECV_SIZE];
	int		len = 0;
	int		k;

	do {
		k = ::recv(fd, buffer, RECV_SIZE - 1, 0);
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

int FT::read(int fd, std::string &out) {
	std::ostringstream stream;
	char	buffer[RECV_SIZE];
	int		len = 0;
	int		k;

	do {
		k = ::read(fd, buffer, RECV_SIZE - 1);
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

std::string FT::itoa(int n) {
	std::ostringstream temp;
	temp << n;
	return (temp.str());
}

bool FT::is_directory(const std::string &path) {
	struct stat	out;
	if (stat(path.c_str(), &out) < 0) {
		std::cerr << "stat() failed: " << strerror(errno) << "\n";
		return (false);
	}
	return ((out.st_mode & S_IFDIR) != 0);
}

bool FT::is_file(const std::string &path) {
	struct stat	out;
	if (stat(path.c_str(), &out) < 0) {
		std::cerr << "stat() failed: " << strerror(errno) << "\n";
		return (false);
	}
	return ((out.st_mode & S_IFREG) != 0);
}