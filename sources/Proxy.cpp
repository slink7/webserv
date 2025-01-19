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
	if(it->fd >= 0)
		close(it->fd);
	return (fds.erase(it));
}

void Proxy::CloseFDs() {
	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
		if (it->fd >= 0)
			close(it->fd);
}

void Proxy::Run() {
	AddSocketsToPoll();
	
	bool exit = false;
	while (!exit) {
		int count = poll(fds.data(), fds.size(), 100000);
		Log::out(Log::DEBUG) << "poll: " << count << " event(s) to treat\n";
		if (count < 0) {
			Log::out(Log::FUNCTION) << "poll() failed: " << strerror(errno) << "\n";
			exit = true;
		}

		for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end() && count > 0; it++) {
			if (!it->revents)
				continue ;
			count--;

			Log::out(Log::INFO) << "fd: " << it->fd << ": " << it->revents << " " << ((it->revents & POLLIN) ? "POLLIN" : "") << " " << ((it->revents & POLLOUT) ? "POLLOUT" : "") << "\n";
			if (!it->fd) {
				exit = true;
				Log::out(Log::INFO) << "STDIN exiting...\n";
			} else if (std::find(sockets.begin(), sockets.end(), it->fd) != sockets.end()) {
				int client_fd = accept(it->fd, 0, 0);
				Log::out(Log::INFO) << "Accepting new client " << client_fd << "\n";
				if (client_fd < 0) {
					Log::out(Log::FUNCTION) << "accept() failed: " << strerror(errno) << "\n";
					exit = true;
				}
				AddFD(client_fd, POLLIN);
				parents[client_fd] = it->fd;
			} else if (it->revents & POLLIN) {
				
				HTTP::Request& req = requests[it->fd];
				req.Receive(it->fd);
				req.Print();
				if (req.GetStartLine().size() < 2) {
					Log::out(Log::ERROR) << "Weird start line\n";
					exit = true;
				} else if (req.GetMethod() == HTTP::INVALID) {
					Log::out(Log::ERROR) << "Invalid method\n";
					exit = true;
				}

				it->revents = 0;
				it->events |= POLLOUT;
			}
			if (it->revents & POLLOUT && requests.find(it->fd) != requests.end()) {
				Log::out(Log::INFO) << "res " << requests[it->fd].GetStartLine() << "\n";
				HTTP::Response res;
				res.SetError(404);
				res.Send(it->fd);
				it->revents = 0;
				
				Log::out(Log::DEBUG) << "Removing fd " << it->fd << "\n";
				it = RemoveClient(it);
			} else {
				it++;
			}
		}
		
	}

	CloseFDs();
}
