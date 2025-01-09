#include "Response.hpp"

std::map<int, Error> HTTP::Response::error_list;

HTTP::Response::Response() :
	Message()
{}

void HTTP::Response::SetStatus(const std::string &status) {
	start_line = "HTTP/1.1 " + status;
}

void HTTP::Response::SetBodyRaw(const std::string &body) {
	this->body = body;
}

bool HTTP::Response::SetBodyFromFile(const std::string &path) {
	std::ifstream file(path.c_str());

	if (!file.is_open()) {
		Log::out(Log::ERROR) << "File \"" << path << "\" couldn't be oppenned\n";
		return (false);
	}

	body.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	return (true);
}

void HTTP::Response::Send(int fd) const {
	std::ostringstream buffer;

	buffer << start_line << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++)
		buffer << it->first << ": " << it->second << "\r\n";
	buffer << "\r\n";
	buffer << body;

	std::string f = buffer.str();
	FT::send(fd, f);
}

void HTTP::Response::ReadCGI(int fd) {
	std::string raw;
	int index = 0;

	FT::read(fd, raw);

	FT::replace(raw, "\r\n", "\n");

	//READING START_LINE
	{
		std::size_t nl = raw.find_first_of('\n');
		if (nl == std::string::npos)
			return ;
		std::size_t sc = raw.find_first_of(':');
		if (nl < sc) {
			start_line = raw.substr(0, nl);
			index = nl + 1;
		}
	}
	
	//CHECKING FOR EMPTY LINE
	std::size_t body_start = raw.find("\n\n", index);
	if (body_start == std::string::npos) {
		Log::out(Log::FUNCTION) << "Error: Missing empty line in request\n";
		return ;
	}
	body_start += 2;

	//READING HEADERS
	{
		while (true) {
			std::size_t name_end = raw.find_first_of(':', index);
			std::size_t value_begin = raw.find_first_not_of(' ', name_end + 1);
			std::size_t newline = raw.find_first_of('\n', index);
			
			if (newline + 2 > body_start)
				break ;
			if (newline <= name_end)
				continue ;
			AddHeader(raw.substr(index, name_end - index), raw.substr(value_begin, newline - value_begin));
			index = newline + 1;
		}
	}

	//READING BODY
	body = raw.substr(body_start);
}

void HTTP::Response::SetError(int error_code) {
	std::map<int, Error>::iterator it = error_list.find(error_code);

	if (it == error_list.end()) {
		Log::out(Log::ERROR) << "Invalid error code " << error_code << "\n";
		if (error_code != 500)
			SetError(500);
		return ; 
	}
	Error& err = it->second;

	SetStatus(FT::itoa(err.code) + " " + err.status);
	bool custom = SetBodyFromFile(err.custom_path);
	if (custom) {
		AddHeader("Content-Type", FT::get_mime_type(err.custom_path));
	} else {
		if (!SetBodyFromFile(err.default_path)) {
			Log::out(Log::ERROR) << "Invalid default path \"" << err.default_path << "\"\n";
			if (error_code != 500)
				SetError(500);
			return ;
		}
		AddHeader("Content-Type", FT::get_mime_type(err.default_path));
	}
	AddHeader("Content-Length", FT::itoa(body.size()));
}

void HTTP::Response::InitErrorList() {
	error_list[400] = Error(400, "Bad Request", "site/errors/400.html", "");
	error_list[401] = Error(401, "Unauthorized", "site/errors/401.html", "");
	error_list[402] = Error(402, "Payment Required", "site/errors/402.html", "");
	error_list[403] = Error(403, "Forbidden", "site/errors/403.html", "");
	error_list[404] = Error(404, "Not Found", "site/errors/404.html", "");
	error_list[405] = Error(405, "Method Not Allowed", "site/errors/405.html", "");
	error_list[406] = Error(406, "Not Acceptable", "site/errors/406.html", "");
	error_list[500] = Error(500, "Internal Server Error", "site/errors/500.html", "");
	error_list[501] = Error(501, "Not Implemented", "site/errors/501.html", "");
	error_list[502] = Error(502, "Bad Gateway", "site/errors/502.html", "");
	error_list[503] = Error(503, "Service Unavailable", "site/errors/503.html", "");
	error_list[504] = Error(504, "Gateway Timeout", "site/errors/504.html", "");
	error_list[505] = Error(505, "HTTP Version Not Supported", "site/errors/505.html", "");
}
