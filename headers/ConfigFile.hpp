/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:20:33 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/13 15:48:54 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include "CGI.hpp"

#include "Method.hpp"
#include "ConfigGroup.hpp"
#include "Config.hpp"

class ConfigFile
{

private:
public:
	std::map<unsigned short, ConfigGroup> port_config;
	std::vector<Config> config_list;
	std::string _content;
	
	ConfigFile(const std::string& path);
	~ConfigFile();
	void startconfig(std::string _content);
	void	LoadFromFile(const std::string& path);
};

#endif