#include "Message.hpp"

const std::string &HTTP::Message::GetVersion() const {
	int	begin = start_line.find("HTTP/");
	int end = start_line.find_first_of(" \n\t\0", begin);
	return (start_line.substr(begin, end - begin));
}

const std::string &HTTP::Message::GetStartLine() const {
	return (start_line);
}

void HTTP::Message::AddHeader(const std::string &name, const std::string &value) {
	headers.insert({name, value});
}

const std::string &HTTP::Message::GetHeaderValue(const std::string &name) const {
	return (headers.find(name)->second);
}

const std::map<std::string, std::string> &HTTP::Message::GetHeaderMap() const {
	return (headers);
}

const std::string &HTTP::Message::GetBody() const {
	return (body);
}