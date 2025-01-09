#include "Request.hpp"

HTTP::Request::Request() :
	Message(),
	method(UNDEFINED),
	target()
{}

void HTTP::Request::Receive(int fd) {
	std::string raw;

	FT::receive(fd, raw);
	FT::replace(raw, "\r\n", "\n");
	
	int index = 0;

	{
		std::size_t end = raw.find_first_of('\n');
		if (end == std::string::npos){
			Log::out(Log::WARNING) << "Warning: No newline in request\n";
			return ;
		}
		start_line = raw.substr(0, end);
		index = end + 1;
	}
	
	std::size_t body_start = raw.find("\n\n", index) + 2;
	if (body_start == std::string::npos) {
		Log::out(Log::WARNING) << "Warning: Missing empty line in request\n";
		return ;
	}

	{
		while (true) {
			std::size_t name_end = raw.find_first_of(':', index);
			std::size_t value_begin = raw.find_first_not_of(' ', name_end + 1);
			std::size_t newline = raw.find_first_of('\n', index);
			if (newline + 2 >= body_start)
				break ;
			if (newline <= name_end)
				continue ;
			AddHeader(raw.substr(index, name_end - index), raw.substr(value_begin, newline - value_begin));
			index = newline + 1;
		}
	}

	{
		body = raw.substr(body_start);
	}

	{
		method = EvaluateMethod(start_line);
		if (method == INVALID)
			Log::out(Log::WARNING) << "Warning: Invalid Method in request\n";
	}

	{
		int begin = start_line.find_first_of(' ') + 1;
		int end = start_line.find_last_of(' ');
		target = start_line.substr(begin, end - begin);
	}
}

HTTP::Method	HTTP::Request::GetMethod() const {
	return (method);
}

const std::string& HTTP::Request::GetTarget() const {
	return (target);
}