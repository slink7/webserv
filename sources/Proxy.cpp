#include "Proxy.hpp"

const int Proxy::max_fds_count = 100;

Proxy::Proxy() {
	fds.reserve(max_fds_count);
}

bool Proxy::AddGroup(ConfigGroup &config)
{
	for (std::vector<unsigned short>::iterator it = config.port.begin(); it != config.port.end(); it++)
		if (!AddSocket(config, *it))
			return (false);
	return (true);
}

bool Proxy::AddSocket(ConfigGroup &group, unsigned short port) {
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
	addr.sin_port = htons(port);
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

	Log::out(Log::INFO) << "Openned socket on port " << port << "\n";
	configs[fd] = &group;
	sockets.push_back(fd);
	return (true);
}

void Proxy::AddFD(int fd, int events) {
	if (fds.size() >= max_fds_count) {
		Log::out(Log::WARNING) << "Can't add more fd to Proxy (" << fds.size() << "/" << max_fds_count << ")\n";
		return ;
	}
	struct pollfd temp;
	std::memset(&temp, 0, sizeof(pollfd));
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
	return (it);
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
		
		const int			recv_size = 1024;
		char				buffer[recv_size] = {0};
		int					len;

		len = recv(it->fd, buffer, recv_size - 1, 0);
		if (len >= 0)
			buffer[len] = 0;

		requests[it->fd] += buffer;

		
		it->revents = 0;
		if (len < (recv_size - 1)) {
			Log::out(Log::DEBUG) << it->fd << " finished sending " << requests[it->fd].size() << " bytes\n";
			it->events |= POLLOUT;
			it->events &= ~POLLIN;
		}
	}

	if (it->revents & POLLOUT) {
		
		HTTP::Request req(requests[it->fd]);
		Log::out(Log::DEBUG) << it->fd << " responding to: \n";
		req.Print(HTTP::Message::ALL);

		HTTP::Response res(req, configs[it->fd]->GetConfig(req));
		res.Send(it->fd);
		
		Log::out(Log::DEBUG) << "Removing fd " << it->fd << "\n";
		RemoveClient(it);
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

		for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end();) {
			if (it->fd == -1)
				it = fds.erase(it);
			else
				it++;
		}
	}

	CloseFDs();
	Clear();
}

void	Proxy::Clear() {
	requests.clear();
}