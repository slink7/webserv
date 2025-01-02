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

const std::string& FT::get_mime_type(const std::string& file) {
	static const std::string def = "application/octet-stream";
	static const int type_count = 9;
    static const std::string mime_types[type_count][2] = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".gif", "image/gif"},
        {".json", "application/json"},
        {".txt", "text/plain"},
        {".pdf", "application/pdf"}
    };

	std::string extension = file.substr(file.find_last_of('.'));

	for (int k = 0; k < type_count; k++)
		if (!mime_types[k][0].compare(extension))
			return (mime_types[k][1]);
    return (def);
}