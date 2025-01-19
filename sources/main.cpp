#include <poll.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <map>
#include <algorithm>

#include "Request.hpp"
#include "Response.hpp"
#include "Log.hpp"
#include "FT.hpp"

struct config {
	unsigned short port;
};

std::map<int, config> configs;
std::vector<int> sockets;
std::vector<pollfd> fds;
std::map<int, int> parents;
std::map<int, HTTP::Request> buffer;

void open_socket(const config& config) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
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
	addr.sin_port = htons(config.port);
	addr.sin_addr.s_addr = 0;

	if (bind(fd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(fd);
		Log::out(Log::FUNCTION) << "bind() failed: " << strerror(errno) << "\n";
		return ;
	}

	if (listen(fd, 5) < 0) {
		close(fd);
		Log::out(Log::FUNCTION) << "listen() failed: " << strerror(errno) << "\n";
		return ;
	}

	Log::out(Log::INFO) << "Openned socket on port " << config.port << "\n";
	configs[fd] = config;
	sockets.push_back(fd);
}

void	add_fd(int fd, int events) {
	struct pollfd temp;
	temp.fd = fd;
	temp.events = events;
	temp.revents = 0;
	fds.push_back(temp);
}

void	rm_fd(std::vector<pollfd>::iterator it) {
	parents.erase(it->fd);
	buffer.erase(it->fd);
	close(it->fd);
	fds.erase(it);
}

int	main() {
	HTTP::Response::InitErrorList();

	fds.reserve(100);

	open_socket((config){8080});
	// open_socket((config){8081});

	add_fd(0, POLLIN);
	for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); it++)
		add_fd(*it, POLLIN);

	bool exit = false;
	int k = 16;
	while (!exit && k--) {
		Log::out(Log::DEBUG) << "\n[" << k << "]\n\n";
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
				break ;
			} else if (std::find(sockets.begin(), sockets.end(), it->fd) != sockets.end()) {
				int client_fd = accept(it->fd, 0, 0);
				Log::out(Log::INFO) << "Accepting new client " << client_fd << "\n";
				if (client_fd < 0) {
					Log::out(Log::FUNCTION) << "accept() failed: " << strerror(errno) << "\n";
					exit = true;
					continue ;
				}
				add_fd(client_fd, POLLIN);
				parents[client_fd] = it->fd;
			} else if (it->revents & POLLIN) {
				
				HTTP::Request& req = buffer[it->fd];
				req.Receive(it->fd);
				req.Print();
				if (req.GetStartLine().size() < 2) {
					Log::out(Log::ERROR) << "Weird start line\n";
					exit = true;
					break ;
				} else if (req.GetMethod() == HTTP::INVALID) {
					Log::out(Log::ERROR) << "Invalid method\n";
					exit = true;
					break ;
				}

				it->revents = 0;
				it->events |= POLLOUT;
			}
			if (it->revents & POLLOUT && buffer.find(it->fd) != buffer.end()) {
				Log::out(Log::INFO) << "res " << buffer[it->fd].GetStartLine() << "\n";
				HTTP::Response res;
				res.SetError(404);
				res.Send(it->fd);
				it->revents = 0;
				
				Log::out(Log::DEBUG) << "Removing fd " << it->fd << "\n";
				parents.erase(it->fd);
				buffer.erase(it->fd);
				close(it->fd);
				it = fds.erase(it);
			} else {
				it++;
			}
		}
		
	}

	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
		close(it->fd);

	return (0);
}