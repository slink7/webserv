#include "Response.hpp"

HTTP::Response::Response() :
	Message()
{}

void HTTP::Response::SetStatus(const std::string &status) {
	start_line = "HTTP/1.1 " + status;
}

void HTTP::Response::SetBodyRaw(const std::string &body) {
	this->body = body;
}

void HTTP::Response::SetBodyFromFile(const std::string &path) {
	std::ifstream file("." + path);

	if (!file.is_open()) {
		std::cout << "File couldn't be oppenned\n";
		return ;
	}

	body.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void HTTP::Response::Send(int fd) const {
	std::ostringstream buffer;

	buffer << start_line << "\r\n";
	for (auto it : headers)
		buffer << it.first << ": " << it.second << "\r\n";
	buffer << "\r\n";
	buffer << body;

	std::string f = buffer.str();
	FT::send(fd, f);
}