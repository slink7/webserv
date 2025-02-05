#include "Request.hpp"

HTTP::Request::Request() :
	Message(),
	method(UNDEFINED),
	target()
{}

HTTP::Request::Request(const std::string &str) :
	Message(),
	method(UNDEFINED),
	target()
{
	LoadFromString(str);
}

void HTTP::Request::LoadFromString(const std::string& raw) {

	std::string str = raw;
	FT::replace(str, "\r\n", "\n");
	
	int index = 0;

	{
		std::size_t end = str.find_first_of('\n');
		if (end == std::string::npos){
			Log::out(Log::WARNING) << "Warning: No newline in request\n";
			return ;
		}
		start_line = str.substr(0, end);
		index = end + 1;
	}
	
	std::size_t body_start = str.find("\n\n", index) + 2;
	if (body_start == std::string::npos) {
		Log::out(Log::WARNING) << "Warning: Missing empty line in request\n";
		return ;
	}

	{
		while (true) {
			std::size_t name_end = str.find_first_of(':', index);
			std::size_t value_begin = str.find_first_not_of(' ', name_end + 1);
			std::size_t newline = str.find_first_of('\n', index);
			if (newline + 2 >= body_start)
				break ;
			if (newline <= name_end)
				continue ;
			AddHeader(str.substr(index, name_end - index), str.substr(value_begin, newline - value_begin));
			index = newline + 1;
		}
	}

	{
		body = str.substr(body_start);
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
		FT::trim(target, " \t/");
	}
}

HTTP::Method	HTTP::Request::GetMethod() const {
	return (method);
}

const std::string& HTTP::Request::GetTarget() const {
	return (target);
}