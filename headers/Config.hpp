/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:27:19 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/13 15:52:02 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "Location.hpp"
#include "CGI.hpp"
#include "Method.hpp"

class Config
{
	private :
		std::string					host;
		std::string					server_name;
		std::string					root;
		std::string					index;
		unsigned long				max_body_length;
		std::map<int, std::string>	error_pages;
		std::vector<unsigned short>	ports;
		std::vector<Location>		locations;
		HTTP::CGI					cgi;
	public :
		Config();
		Config(std::string& src);
		~Config();
		void manageLocation(std::string _content);
		std::string& get_host();
		std::vector<unsigned short>& get_port();
		void	Print() const;
};

#endif