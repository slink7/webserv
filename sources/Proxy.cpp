#include "Proxy.hpp"

const int Proxy::max_fds_count = 100;

Proxy::Proxy() {
	fds.reserve(max_fds_count);
}

bool Proxy::AddServer(const Config &config) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		Log::out(Log::FUNCTION) << "socket() failed: " << strerror(errno) << "\n";
		return (false);
	}

	int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        Log::out(Log::FUNCTION) << "setsockopt() failed: " << strerror(errno) << "\n";
		return (false);
	}

	struct sockaddr_in	addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(config.port);
	addr.sin_addr.s_addr = 0;

	if (bind(fd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(fd);
		Log::out(Log::FUNCTION) << "bind() failed: " << strerror(errno) << "\n";
		return (false);
	}

	if (listen(fd, 10) < 0) {
		close(fd);
		Log::out(Log::FUNCTION) << "listen() failed: " << strerror(errno) << "\n";
		return (false);
	}

	Log::out(Log::INFO) << "Openned socket on port " << config.port << "\n";
	configs[fd] = config;
	sockets.push_back(fd);
	return (true);
}

void Proxy::AddFD(int fd, int events) {
	struct pollfd temp;
	temp.fd = fd;
	temp.events = events;
	temp.revents = 0;
	fds.push_back(temp);
}

void Proxy::AddSocketsToPoll() {
	for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); it++)
		AddFD(*it, POLLIN);
}

std::vector<pollfd>::iterator Proxy::RemoveClient(std::vector<pollfd>::iterator it)
{
	parents.erase(it->fd);
	requests.erase(it->fd);
	if(it->fd >= 0) {
		close(it->fd);
		it->fd = -1;
	}
	return (fds.erase(it));
}

void Proxy::CloseFDs() {
	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++) {
		if (it->fd >= 0) {
			close(it->fd);
			it->fd = -1;
		}
	}
}

bool Proxy::HandleEvent(std::vector<pollfd>::iterator& it) {
	if (!it->fd) {

		Log::out(Log::INFO) << "STDIN exiting...\n";
		return (false);

	} else if (std::find(sockets.begin(), sockets.end(), it->fd) != sockets.end()) {
		
		int client_fd = accept(it->fd, 0, 0);
		Log::out(Log::INFO) << "Accepting new client " << client_fd << "\n";
		if (client_fd < 0) {
			Log::out(Log::FUNCTION) << "accept() failed: " << strerror(errno) << "\n";
			return (false);
		}

		AddFD(client_fd, POLLIN);
		parents[client_fd] = it->fd;

	} else if (it->revents & POLLIN) {
		
		const int			recv_size = 16;
		char				buffer[recv_size];
		int					len;

		len = recv(it->fd, buffer, recv_size - 1, 0);
		if (len >= 0)
			buffer[len] = 0;

		requests[it->fd] += buffer;

		Log::out(Log::DEBUG) << "RECV\n";
		Log::out(Log::DEBUG) << "size: " << requests[it->fd].size() << "\n";
		Log::out(Log::DEBUG) << "frag: " << (int)buffer[len - 3] << " " << (int)buffer[len - 2] << " " << (int)buffer[len - 1] << "\n";
		Log::out(Log::DEBUG) << "len:" <<  len << "\n";

		it->revents = 0;
		if (len < (recv_size - 1)) {
			it->events |= POLLOUT;
		}
	}

	if (it->revents & POLLOUT) {
		


		HTTP::Response res;
		res.SetError(404);
		res.Send(it->fd);
		
		Log::out(Log::DEBUG) << "Removing fd " << it->fd << "\n";
		it = RemoveClient(it);
	} else {
		it++;
	}
	return (true);
}

void Proxy::Run() {
	AddSocketsToPoll();
	
	bool exit = false;
	while (!exit) {
		int count = poll(fds.data(), fds.size(), -1);
		if (count < 0) {
			Log::out(Log::FUNCTION) << "poll() failed: " << strerror(errno) << "\n";
			exit = true;
		}

		for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end() && count > 0; it++) {
			if (!it->revents)
				continue ;
			count--;

			if(!HandleEvent(it))
				exit = true;
		}
		
	}

	CloseFDs();
}
