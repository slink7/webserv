/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:20:33 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/02 10:00:36 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

// #include "CGI.hpp"

// #include "Method.hpp"
// #include "ConfigGroup.hpp"

class ConfigFile
{

private:
	// std::vector<ConfigGroup> groups;
	std::string _content;
public:
	ConfigFile();
	~ConfigFile();
	void startconfig(std::string _content);
	std::string readFileToString(const std::string& filePath);
	// ConfigFile(const std::string& path);
	// bool	LoadFromFile(const std::string& path);
};

#endif