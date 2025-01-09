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

	cgi.add(".php", "/usr/bin/php-cgi");

	HTTP::Response::InitErrorList();
}

bool pred(const pollfd& poll) {
	return (poll.fd == -1);
}

void Server::start() {
	while (running) {

		int	count = poll(fds.get_data(), fds.get_size(), -1);
		if (count < 0) {
			Log::out(Log::FUNCTION) << "poll() failed: " << strerror(errno) << "\n";
			running = false;
			break ;
		}

		for (int k = 0; k < fds.get_size() && count > 0; k++) {
			if (handle_event(fds.at(k)))
				count--;
		}

		fds.compact(pred);
	}
	Log::out(Log::INFO) << "Closing " << fds.get_size() << " fds\n";
	for (int k = 0; k < fds.get_size(); k++) {
		Log::out(Log::DEBUG) << "\tclosed(" << fds.at(k).fd << ") B \n";
		close(fds.at(k).fd);
		fds.at(k).fd = -1;
	}
}

bool Server::handle_event(pollfd &fd)
{
	if (!fd.revents)
		return (false);

	if (fd.fd == 0) {
		Log::out(Log::INFO) << "STDOUT\n";
		running = false;
	}

	else if (fd.fd == socket.get_fd()) {

		int client_fd = accept(fd.fd, 0, 0);
		if (client_fd < 0) {
			if (errno != EWOULDBLOCK) {
				Log::out(Log::FUNCTION) << "accept() failed: " << strerror(errno) << "\n";
				running = false;
			}
		}
		Log::out(Log::DEBUG) << "\topenned(" << client_fd << ")\n";
		Socket::set_flag(client_fd, O_NONBLOCK, true);
		struct pollfd temp;
		temp.fd = client_fd;
		temp.events = POLLIN;
		temp.revents = 0;
		fds.add(temp);

	} else {

		if (fd.revents == POLLIN) {
			
			Log::out(Log::INFO) << "\tReceiving from " << fd.fd << "\n";

			HTTP::Request req;
			req.Receive(fd.fd);

			req.Print(HTTP::Request::START_LINE);

			if (cgi.handle(req, fd.fd))
				Log::out(Log::INFO) << "CGI handled\n";
			else 
				handle_request(req, fd.fd);

			Log::out(Log::DEBUG) << "\tclosed(" << fd.fd << ") A \n";
			close(fd.fd);
			fd.fd = -1;

		}
	}
	return false;
}

void Server::handle_request(HTTP::Request& req, int fd)
{
	HTTP::Response rep;

	if (req.GetMethod() == HTTP::UNDEFINED) {
		rep.SetError(500);
	} else if (req.GetMethod() == HTTP::INVALID) {
		rep.SetError(501);
	} else if (FT::is_directory(req.GetTarget().substr(1))) {
		rep.SetError(403);
	} else if (rep.SetBodyFromFile(req.GetTarget().substr(1))) {
		rep.SetStatus("200 OK");
		rep.AddHeader("Content-Type", FT::get_mime_type(req.GetTarget()));
		rep.AddHeader("Content-Length", FT::itoa(rep.GetBody().size()));
	} else {
		rep.SetError(404);
	}
	rep.Print(HTTP::Request::NO_BODY);
	rep.Send(fd);
}
