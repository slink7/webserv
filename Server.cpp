#include "Server.hpp"

Server::Server(int port) :
	running(true),
	socket(port, 5)
{
	if (!socket.is_enabled()) {
		running = false;
		return ;
	}

	socket.set_flag(O_NONBLOCK, true);

	fds.add({socket.get_fd(), POLLIN, 0});
	
}

void Server::start() {
	while (running) {

		int	count = poll(fds.get_data(), fds.get_size(), -1);
		if (count < 0) {
			std::cout << "poll() failed: " << strerror(errno) << "\n";
			running = false;
			break ;
		}

		for (int k = 0; k < fds.get_size() && count > 0; k++) {
			if (handle_event(fds.at(k)))
				count--;
		}

		fds.compact([](const pollfd& poll) -> bool {
			return (poll.fd == -1);
		});
	}
}

bool Server::handle_event(pollfd &fd)
{
	if (!fd.revents)
		return (false);
	
	if (fd.fd == socket.get_fd()) {

		int client_fd = accept(fd.fd, 0, 0);
		if (client_fd < 0) {
			if (errno != EWOULDBLOCK) {
				std::cerr << "accept() failed: " << strerror(errno) << "\n";
				running = false;
			}
		}
		fds.add({client_fd, POLLIN, 0});

	} else {

		if (fd.revents == POLLIN) {
			
			std::cout << "\tReceiving from " << fd.fd << "\n";

			char buffer[RECV_SIZE] = {0};
			int r = recv(fd.fd, buffer, sizeof(buffer) - 1, 0);
			if (r < 0) {
				if (r != EWOULDBLOCK) {
					std::cout << "recv() failed: " << strerror(errno) << "\n";
					running = false;
				}
			}
			running = !handle_request(buffer, fd.fd);
			if (r < RECV_SIZE) {
				close(fd.fd);
				fd.fd = -1;
			}

		}
	}
	return false;
}

void	send_403(int fd) {
	std::string r403 =
		"HTTP/1.1 403 Forbidden\n"
		"Server: Webserv\n"
		"Content-Type: text/html\n"
		"Content-Length: 87\n"
		"Connection: keep-alive\n"
		"\n"
		"<html>\n"
		"<head><title>403</title></head>\n"
		"<body><h1>403 Forbidden...</h1></body>\n"
		"</html>\n";
}

void	send_404(int fd) {
	std::string r404 = 
		"HTTP/1.1 404 Not Found\n"
		"Server: Webserv\n"
		"Content-Type: text/html\n"
		"Content-Length: 87\n"
		"Connection: keep-alive\n"
		"\n"
		"<html>\n"
		"<head><title>404</title></head>\n"
		"<body><h1>404 Not Found...</h1></body>\n"
		"</html>\n";
	int e = send(fd, r404.c_str(), r404.size(), 0);
	if (e < 0) {
		std::cerr << "send() failed: " << strerror(errno) << "\n";
	}
}

bool Server::handle_request(char *str, int fd)
{
	std::cout << "Handling request\n";

	if (!strncmp(str, "STOP", 4))
		return (true);
	if (strncmp(str, "GET ", 4))
		return (false);

	*strchr(str + 5, ' ') = 0;

	if (str[strlen(str) - 1] == '/' || !access(str + 5, F_OK | R_OK)) {
		send_403(fd);
		return (false);
	}
	std::ifstream file(str + 5);
	if (!file.is_open()) {
		send_404(fd);
		return (false);
	} else {
		std::cout << "Sending " << (str + 5) << "\n";

		std::ostringstream f;
		f << file.rdbuf();

		std::string sf = f.str();

		char *c = strrchr(str + 5, '.');

		std::ostringstream ss;
		ss << "HTTP/1.1 200 OK\n";
		ss << "Server: Webserv\n";
		ss << "Content-Type: text/" << c + 1 << "\n";
		ss << "Connection: keep-alive\n";
		ss << "Content-Length: " << sf.size() << "\n";
		ss << "\n";

		ss << sf;
		//ss << file.rdbuf();

		std::string s = ss.str();
		std::cout << s << "\n";
		int e = send(fd, s.c_str(), s.size(), 0);
		std::cout << "Sent " << e << "b/" << s.size() << "b on fd " << fd << "\n";
	}
	return (false);
}
