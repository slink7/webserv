#include "Server.hpp"

Server::Server(int port) :
	running(true),
	socket(port, 5)
{
	if (!socket.is_enabled()) {
		running = false;
		return ;
	}

	socket.set_flag(O_NONBLOCK, false);

	fds.add({0, POLLIN, 0});
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
	for (int k = 0; k < fds.get_size(); k++) {
		std::cout << "\tclosed(" << fds.at(k).fd << ")\n";
		close(fds.at(k).fd);
		fds.at(k).fd = -1;
	}
}

bool Server::handle_event(pollfd &fd)
{
	if (!fd.revents)
		return (false);

	if (fd.fd == 0) {
		std::cout << "STDOUT\n";
		running = false;
	}

	else if (fd.fd == socket.get_fd()) {

		int client_fd = accept(fd.fd, 0, 0);
		if (client_fd < 0) {
			if (errno != EWOULDBLOCK) {
				std::cerr << "accept() failed: " << strerror(errno) << "\n";
				running = false;
			}
		}
		std::cout << "\topenned(" << client_fd << ")\n";
		// Socket::set_flag(client_fd, O_NONBLOCK, true);
		fds.add({client_fd, POLLIN, 0});

	} else {

		if (fd.revents == POLLIN) {
			
			std::cout << "\n\tReceiving from " << fd.fd << "\n";

			std::string request;
			int r = msg::receive(fd.fd, request);
			running = !handle_request(request, fd.fd);
			std::cout << "\tclosed(" << fd.fd << ")\n";
			close(fd.fd);
			fd.fd = -1;

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
	msg::send(fd, r404);
}

bool Server::handle_request(std::string& req, int fd)
{
	std::cout << "\n\tHandling request:\n";

	if (!req.compare("STOP")) {
		std::cout << "STOP request\n";
		return (true);
	}
	if (!req.compare("GET ")) {
		std::cout << "GET resquest\n";	
		return (false);
	}

	req = req.substr(0, req.find(' ', 5));

	if (req[req.size() - 1] == '/' || access(req.c_str() + 5, F_OK | R_OK)) {
		std::cout << "Sending 403\n";
		send_403(fd);
		return (false);
	}
	std::ifstream file(req.c_str() + 5);
	if (!file.is_open()) {
		std::cout << "Sending 404\n";
		send_404(fd);
		return (false);
	} else {
		std::cout << "Sending " << (req.c_str() + 5) << "\n";

		std::ostringstream f;
		f << file.rdbuf();

		std::string sf = f.str();

		int ip = req.find_last_of('.');

		std::ostringstream ss;
		ss << "HTTP/1.1 200 OK\n";
		ss << "Server: Webserv\n";
		ss << "Content-Type: text/" << req.c_str() + ip + 1 << "\n";
		ss << "Connection: keep-alive\n";
		ss << "Content-Length: " << sf.size() << "\n";
		ss << "\n";

		ss << sf;
		//ss << file.rdbuf();

		std::string s = ss.str();
		if (s.size() < 256)
			std::cout << s << "\n";
		int e = msg::send(fd, s);
		std::cout << "Sent " << e << "b/" << s.size() << "b on fd " << fd << "\n";
	}
	return (false);
}
