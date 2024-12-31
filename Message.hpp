#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>
#include <map>

namespace HTTP {
	class Message {
	
	public:
		Message() = delete;
		Message(const Message& other) = delete;

		Message&	operator=(const Message& other) = delete;

		std::string	GetVersion() const;
		const std::string&	GetStartLine() const;

		void	AddHeader(const std::string& name, const std::string& value);
		const std::string&	GetHeaderValue(const std::string& name) const;
		const std::map<std::string, std::string>&	GetHeaderMap() const;

		const std::string&	GetBody() const;

	protected:
		std::string	start_line;
		std::map<std::string, std::string>	headers;
		std::string	body;
	};
};

#endif