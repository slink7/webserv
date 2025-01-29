#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Message.hpp"
#include "Method.hpp"
#include "FT.hpp"

namespace HTTP {
	class Request : public Message {

	public:
		Request();
		Request(const std::string& str);

		void		LoadFromString(const std::string& str);
		Method		GetMethod() const;
		const std::string&	GetTarget() const;

	private:
		Method		method;
		std::string	target;
	};
}

#endif