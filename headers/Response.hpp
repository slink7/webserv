#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <fstream>
#include <sstream>

#include "FT.hpp"
#include "Message.hpp"
#include "Request.hpp"
#include "Error.hpp"
#include "Log.hpp"
#include "Config.hpp"
#include "ConfigGroup.hpp"

namespace HTTP {

	class Response : public Message {
	
	public:
		Response(Config const* conf);
		Response(const HTTP::Request& req, Config const* conf);

		void		SetStatus(const std::string& status);
		void		SetBodyRaw(const std::string& body);
		bool		SetBodyFromFile(const std::string& path);
		void		LoadFromRequest(const HTTP::Request& req);
		void		Send(int fd) const;
		void		ReadCGI(int fd);
		void		SetError(int error_code);
		static void	InitErrorList();
	
	private:
		const Config* conf;
		static std::map<int, Error> error_list;
	};
}

#endif