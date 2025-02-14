#include "FT.hpp"

int FT::send(int fd, std::string &msg, int len) {
	return (send(fd, msg.c_str(), len));
}

int FT::send(int fd, const char *msg, int len) {
	int	out;

	out = ::send(fd, msg, len, 0);
	if (out < 0) {
		Log::out(Log::FUNCTION) << "send() failed: " << strerror(errno) << "\n";
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
			if (errno != EWOULDBLOCK) {
				Log::out(Log::FUNCTION) << "recv() failed: " << strerror(errno) << "\n";
			}
			out = stream.str();
			break ;
		}
		buffer[k] = 0;
		len += k;
		stream << buffer;
	} while (k >= RECV_SIZE - 1);
	out = stream.str();
	return (len);
}

int FT::read(int fd, std::string &out) {
	std::ostringstream stream;
	char	buffer[RECV_SIZE];
	int		len = 0;
	int		k;

	do {
		k = ::read(fd, buffer, RECV_SIZE - 1);
		if (k < 0) {
			Log::out(Log::FUNCTION) << "recv() failed: " << strerror(errno) << "\n";
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
		Log::out(Log::FUNCTION) << "stat() failed: " << strerror(errno) << "\n";
		return (false);
	}
	return ((out.st_mode & S_IFDIR) != 0);
}

bool FT::is_file(const std::string &path) {
	struct stat	out;
	if (stat(path.c_str(), &out) < 0) {
		Log::out(Log::FUNCTION) << "stat() failed: " << strerror(errno) << "\n";
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

	std::size_t ext_pos = file.find_last_of('.');
	if (ext_pos == std::string::npos)
		return (def);
	std::string extension = file.substr(ext_pos);

	for (int k = 0; k < type_count; k++)
		if (!mime_types[k][0].compare(extension))
			return (mime_types[k][1]);
    return (def);
}

void FT::replace(std::string &str, const char *from, const char *to) {
	std::size_t pos = 0;
	while ((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, std::strlen(from), to);
	}
}

void FT::trim(std::string &str, const std::string &set) {
	std::size_t start = str.find_first_not_of(set);
	if (start == std::string::npos)
		return ;
	std::size_t end = str.find_last_not_of(set);
	if (end == std::string::npos)
		return ;
	str = str.substr(start, end - start + 1);
}

std::string FT::get_value(const std::string& source, const std::string& name, const std::string& limit) {
    std::string res = "";
    size_t start = source.find(name);
	
    if (start == std::string::npos)
		return (res);

	start = source.find_first_not_of(" \t\n", start + name.length());
	size_t end = source.find(limit, start);

	if (end == std::string::npos)
		return (res);
		
	res = source.substr(start, end - start);
        
    return (res);
}

int	FT::get_int(const std::string& source, const std::string& name) {
	return (std::atoi(get_value(source, name, ";").c_str()));
}

bool FT::get_file(std::string &dst, const std::string &path) {
	if (FT::is_directory(path)) {
		Log::out(Log::ERROR) << "Tried to open a directory : \"" << path << "\"\n";
		return (false);
	}
	
	std::ifstream file(path.c_str());

	if (!file.is_open()) {
		Log::out(Log::WARNING) << "File \"" << path << "\" couldn't be openned\n";
		return (false);
	}

	dst.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	return (true);
}

int FT::count(const std::string &src, const std::string &word) {
	int	out = 0;

    size_t pos = src.find(word);
    while (pos != std::string::npos) {
        out++;
        pos = src.find(word, pos + word.length());
    }
	return (out);
}
