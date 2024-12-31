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

	struct pollfd temp;
	temp.fd = 0;
	temp.events = POLLIN;
	temp.revents = 0;
	fds.add(temp);
	temp.fd = socket.get_fd();
	temp.events = POLLIN;
	temp.revents = 0;
	fds.add(temp);

	// cgi.add(".php", "/usr/bin/php-cgi");
}

bool pred(const pollfd& poll) {
	return (poll.fd == -1);
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

		fds.compact(pred);
	}
	std::cout << "Closing " << fds.get_size() << " fds\n";
	for (int k = 0; k < fds.get_size(); k++) {
		std::cout << "\tclosed(" << fds.at(k).fd << ") B \n";
		close(fds.at(k).fd);
		fds.at(k).fd = -1;
	}
}

std::string getMimeType(const std::string& file) {
	static const int type_count = 9;
    static const std::string mimeTypes[type_count][2] = {
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

	for (int k = 0; k < type_count; k++)
		if (!mimeTypes[k][0].compare(file.substr(file.find_last_of('.'))))
			return (mimeTypes[k][1]);
    return ("application/octet-stream");
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
		struct pollfd temp;
		temp.fd = client_fd;
		temp.events = POLLIN;
		temp.revents = 0;
		fds.add(temp);

	} else {

		if (fd.revents == POLLIN) {
			
			std::cout << "\n\tReceiving from " << fd.fd << "\n";

			HTTP::Request req;
			req.Receive(fd.fd);

			std::cout << "\n\tREQUEST\n";
			std::cout << "Method:" << req.GetMethod() << " \n";
			req.Print(HTTP::Request::NO_BODY);

			HTTP::Response rep;

			if (req.GetMethod() == HTTP::UNDEFINED) {
				rep.SetStatus("500 Internal Server Error");
				rep.SetBodyFromFile("site/errors/500.html");
				rep.AddHeader("Content-Type", getMimeType("site/errors/500.html"));
			} else if (req.GetMethod() == HTTP::INVALID) {
				rep.SetStatus("501 Not Implemented");
				rep.SetBodyFromFile("site/errors/501.html");
				rep.AddHeader("Content-Type", getMimeType("site/errors/501.html"));
			} else if (FT::is_directory(req.GetTarget().substr(1))) {
				rep.SetStatus("403 Forbidden");
				rep.SetBodyFromFile("site/errors/403.html");
				rep.AddHeader("Content-Type", getMimeType("site/errors/403.html"));
						
			} else if (rep.SetBodyFromFile(req.GetTarget().substr(1))) {
				rep.SetStatus("200 OK");
				rep.AddHeader("Content-Type", getMimeType(req.GetTarget()));

			} else {
				rep.SetStatus("404 Not found");
				if (!rep.SetBodyFromFile("site/errors/404.html"))
					std::cerr << "Bruh 404 \n";
				rep.AddHeader("Content-Type", getMimeType("site/errors/404.html"));
			}
			rep.AddHeader("Content-Length", FT::itoa(rep.GetBody().size()));
			std::cout << "\n\tRESPONSE\n";
			rep.Print(HTTP::Request::NO_BODY);
			rep.Send(fd.fd);
			

			std::cout << "\tclosed(" << fd.fd << ") A \n";
			close(fd.fd);
			fd.fd = -1;

		}
	}
	return false;
}

void Server::handle_request(std::string& req, int fd)
{
	(void) req;
	(void) fd;
}
