#include "Directory.hpp"

void Directory::AddClient(int fd, int socket) {
	parents[fd] = socket;

	pollfd client = {};
	client.fd = fd;
	client.events = 0;
	client.revents = POLLIN;
	fd_events.push_back(client);


}

void Directory::HandleEvents() {

	int count = poll(fd_events.data(), fd_events.size(), -1);
	if (count < 0) {
		Log::out(Log::FUNCTION) << "poll() failed: " << strerror(errno) << "\n";
		return ;
	}

	for (std::vector<pollfd>::iterator it; it != fd_events.end() && count; it++) {
		if (!it->revents)
			continue ;
		count--;
		if (!it->fd) {
			Log::out(Log::INFO) << "STDIN\n";
			std::string str;
			FT::read(0, str);

		} else if (IsSocket(it->fd)) {
			int client_fd = accept(it->fd, 0, 0);
			if (client_fd < 0) {
				Log::out(Log::FUNCTION) << "accept() failed: " << strerror(errno) << "\n";
				continue ;
			}
			AddClient(client_fd, it->fd);
		}

	}
}

bool Directory::IsSocket(int fd) {
	return (std::find(sockets.begin(), sockets.end(), fd) != sockets.end());
}