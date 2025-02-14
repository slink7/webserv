#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "Log.hpp"

namespace HTTP {
	class Message {
	
	private:
		Message(const Message& other);

		Message&	operator=(const Message& other);

	public:

		std::string	GetVersion() const;
		const std::string&	GetStartLine() const;

		void	AddHeader(const std::string& name, const std::string& value);
		const std::string&	GetHeaderValue(const std::string& name) const;
		const std::map<std::string, std::string>&	GetHeaderMap() const;

		const std::string&	GetBody() const;

		enum	PrintFlag {
			START_LINE = 1 << 0,
			HEADERS = 1 << 1,
			BODY = 1 << 2,
			NO_BODY = START_LINE | HEADERS,
			ALL = START_LINE | HEADERS | BODY
		};

		void	Print(PrintFlag flags = START_LINE) const;

	protected:
		std::string	start_line;
		std::map<std::string, std::string>	headers;
		std::string	body;

		Message();
	};
};

#endif