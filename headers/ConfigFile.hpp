/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:20:33 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/04 17:00:16 by ellehmim         ###   ########.fr       */
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
	std::map<unsigned short, ConfigGroup> port_config;
	std::vector<Config> config_list;
	std::string _content;
	std::vector<unsigned short>	port;
public:
	ConfigFile();
	~ConfigFile();
	void startconfig(std::string _content);
	std::string readFileToString(const std::string& filePath);
};

#endif