#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <fstream>
#include <sstream>

#include "FT.hpp"
#include "Message.hpp"

namespace HTTP {

	class Response : public Message {
	private:
		Response(const Response& other);

		Response&	operator=(const Response& other);
	
	public:
		Response();

		void	SetStatus(const std::string& status);
		void	SetBodyRaw(const std::string& body);
		void	SetBodyFromFile(const std::string& path);
		void	Send(int fd) const;
	};

}

#endif