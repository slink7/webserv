#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Message.hpp"
#include "Methods.hpp"
#include "FT.hpp"

namespace HTTP {
	class Request : public Message {

	public:
		Request();

		void		Receive(int fd);
		Method		GetMethod() const;
		const std::string&	GetTarget() const;

	private:
		Method		method;
		std::string	target;
	};
}

#endif