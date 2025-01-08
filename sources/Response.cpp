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
		std::cout << "File \"" << path << "\" couldn't be oppenned\n";
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
		std::cerr << "Error: Missing empty line in request\n";
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

	// Log::out(Log::DEBUG) << (it != error_list.end() ? "true" : "false") << "\n";

	if (it == error_list.end()) {
		Log::out(Log::ERROR) << "Invalid error code " << error_code << "\n";
		if (error_code != 500)
			SetError(500);
		return ; 
	}
	Error err = it->second;

	Log::out(Log::DEBUG) << " " << error_code << " " << err.code << " " << err.status << " " << err.default_path << " " << err.custom_path << "\n"; 

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
	error_list[403] = Error(403, "Forbidden", "site/errors/403.html", "");
	error_list[404] = Error(404, "Not Found ...", "site/errors/404.html", "");
	error_list[500] = Error(500, "Internal Server Error", "site/errors/500.html", "");
	error_list[501] = Error(501, "Not Implemented", "site/errors/501.html", "");
}
