<<<<<<< HEAD
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:05:37 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/02 10:05:38 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

>>>>>>> parsing
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <fstream>
#include <sstream>

#include "FT.hpp"
#include "Message.hpp"
#include "Request.hpp"
#include "Error.hpp"
#include "Log.hpp"
<<<<<<< HEAD
#include "Config.hpp"
#include "ConfigGroup.hpp"
=======
>>>>>>> parsing

namespace HTTP {

	class Response : public Message {
	
	public:
<<<<<<< HEAD
		Response(Config const* conf);
		Response(const HTTP::Request& req, Config const* conf);
=======
		Response();
		Response(const HTTP::Request& req);
>>>>>>> parsing

		void		SetStatus(const std::string& status);
		void		SetBodyRaw(const std::string& body);
		bool		SetBodyFromFile(const std::string& path);
		void		LoadFromRequest(const HTTP::Request& req);
		void		Send(int fd) const;
		void		ReadCGI(int fd);
		void		SetError(int error_code);
		static void	InitErrorList();
	
	private:
<<<<<<< HEAD
		const Config* conf;
=======
>>>>>>> parsing
		static std::map<int, Error> error_list;
	};
}

#endif