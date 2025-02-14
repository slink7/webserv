#include "Message.hpp"

HTTP::Message::Message() :
	start_line(),
	headers(),
	body()
{}

std::string HTTP::Message::GetVersion() const {
	int	begin = start_line.find("HTTP/");
	int end = start_line.find_first_of(" \n\t", begin);
	return (start_line.substr(begin, end - begin));
}

const std::string &HTTP::Message::GetStartLine() const {
	return (start_line);
}

void HTTP::Message::AddHeader(const std::string &name, const std::string &value) {
	headers.insert(std::pair<std::string, std::string>(name, value));
}

const std::string &HTTP::Message::GetHeaderValue(const std::string &name) const {
	std::map<std::string, std::string>::const_iterator cit = headers.find(name);
	if (cit == headers.end()) {
		static const std::string empty = "";
		return (empty);
	}
	return (cit->second);
}

const std::map<std::string, std::string> &HTTP::Message::GetHeaderMap() const {
	return (headers);
}

const std::string &HTTP::Message::GetBody() const {
	return (body);
}

void HTTP::Message::Print(PrintFlag flags) const {
	std::stringstream ss;
	ss << "\tMESSAGE\n";
	if (flags & START_LINE)
		ss << "Start_line:\"" << GetStartLine() << "\"\n";
	if (flags & HEADERS) {
		ss << "Headers:\n";
		for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++)
			ss << "\t\"" << it->first << "\" \t= \"" << it->second << "\"\n";
	}
	if (flags & BODY)
		ss << "Body:\"\n" << body << "\n\"\n";
	Log::out(Log::INFO) << ss.str();
}
