#include "Socket.hpp"

#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>


#include "CompactList.tpp"

#define RECV_SIZE 1024

int	set_flag(int fd, int flag, bool enabled) {
	int flags = fcntl(fd, F_GETFL);
	return (fcntl(fd, F_SETFL, enabled ? (flags & ~flag) : (flags | flag)));
}

bool	handle_request(char *str, int fd) {
	if (!strncmp(str, "STOP", 4))
		return (true);
	if (strncmp(str, "GET ", 4))
		return (false);

	*strchr(str + 5, ' ') = 0;
	std::ifstream file(str + 5);
	if (!file.is_open()) {
		std::cout << "File not found\n";
		send(fd, "HTTP/1.1 404 Not Found", 23, 0);
	} else {
		std::cout << "Sending " << (str + 5) << "\n";

		std::ostringstream ss;

		ss << "HTTP/1.1 200 OK\n";
		ss << "Content-Length: " << 200 << "\n";

		ss << file.rdbuf();
		//ss << file.rdbuf();

		std::string s = ss.str();
		std::cout << s << "\n";
		send(fd, s.c_str(), s.size(), 0);
	}
	return (false);
}

int	main() {
	Socket sock(8080, 5);

	if (!sock.is_enabled())
		return (1);

	set_flag(sock.get_fd(), O_NONBLOCK, true);

	int on = 1;
	setsockopt(sock.get_fd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));

	
	CompactList<pollfd> fds;
	fds.add({sock.get_fd(), POLLIN, 0});

	bool	stop = false;
	while (!stop) {

		std::cout << "Handling " << fds.get_size() << " fds\n";
		int count = poll(fds.get_data(), fds.get_size(), -1);
		if (count < 0) {
			std::cout << "poll() failed: " << strerror(errno) << "\n";
			stop = true;
		}
		std::cout << "Handling " << count << " from " << fds.get_size() << "\n";
		for (int k = 0; k < fds.get_size() && count > 0; k++) {
			if (fds.at(k).revents == 0)
				continue ;
			
			if (fds.at(k).fd == sock.get_fd()) {
				std::cout << "\tAccepting new clients\n";
				std::cout << "\t\tnew fd: ";
				int client_fd = accept(sock.get_fd(), 0, 0);
				std::cout << client_fd << "\n";
				if (client_fd < 0) {
					if (errno != EWOULDBLOCK) {
						std::cerr << "accept() failed: " << strerror(errno) << "\n";
						stop = true;
					}
				}
				fds.add({client_fd, POLLIN, 0});
				

			} else {

				if (fds.at(k).revents == POLLIN) {
					
					std::cout << "\tReceiving from " << fds.at(k).fd << "\n";

					char buffer[RECV_SIZE] = {0};
					int r = recv(fds.at(k).fd, buffer, sizeof(buffer) - 1, 0);
					if (r < 0) {
						if (r != EWOULDBLOCK) {
							std::cout << "recv() failed: " << strerror(errno) << "\n";
							stop = true;
						}
					}
					if (r < RECV_SIZE) {
						close(fds.at(k).fd);
						fds.at(k).fd = -1;
					}

					stop = handle_request(buffer, fds.at(k).fd);
				}
			}
			count--;
		}
		fds.compact([](const pollfd& poll) -> bool {
			return (poll.fd == -1);
		});
	}
}