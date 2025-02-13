/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:04:57 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/13 15:51:57 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Config.hpp"
#include "../headers/Location.hpp"

Config::Config () {}

static std::vector<unsigned short> porthandler(const std::string& src) {
    std::string buff;
    std::vector<unsigned short> ports;
    const std::string word = "listen ";
    size_t start = src.find(word, 0);
    while (start != std::string::npos)
    {
        start += word.length();
        size_t end = src.find(";", start);
        if (end != std::string::npos)
        {
            buff = src.substr(start, end - start);
            if(std::atoi(buff.c_str()) < 0 || std::atoi(buff.c_str()) > 65535)
                Log::out(Log::ERROR) << "Port " << buff << " is unvalid!\n";
            else
                ports.push_back(std::atoi(buff.c_str()));
        }
        else
        {
            buff = src.substr(start);
            if(std::atoi(buff.c_str()) < 0 || std::atoi(buff.c_str()) > 65535)
                Log::out(Log::ERROR) << "Port " << buff << " is unvalid!\n";
            else
                ports.push_back(std::atoi(buff.c_str()));
            break; 
        } 
        start = src.find(word, end);
    }
    return ports;
}

Config::Config(std::string& src) :
    host(FT::get_value(src, "host ")),
    server_name(FT::get_value(src, "server_name ")),
    root(FT::get_value(src, "root ")),
    index(FT::get_value(src, "index ")),
    max_body_length(FT::get_int(src, "client_max_body_size "))
{
    ports = porthandler(src);
    std::string word = "error_page ";
    std::string buf;
    std::string root_error;
    std::string errornb;
    size_t start = src.find(word);
    while (start != std::string::npos)
    {
        start += word.length();
        size_t end = src.find(";", start);
        if (end != std::string::npos)
        {
            buf = src.substr(start, end - start);
            std::stringstream ss(buf);
            getline(ss, errornb, ' ') && (ss >> root_error);
            error_pages[static_cast<int>(std::atoi(errornb.c_str()))] = root_error;
            start = src.find(word, end);
        }
    }
    manageLocation(src);
}

Config::~Config () {}

void Config::manageLocation(std::string _content) {
    size_t start = 0;
    std::string location_block;
    std::string location_start = "location ";
    std::string location_end = "}";
    while ((start = _content.find(location_start, start)) != std::string::npos)
    {
        size_t end = _content.find(location_end, start);
        if (end != std::string::npos)
        {
            location_block = _content.substr(start, end - start + 1);
            locations.push_back(Location(location_block));
            start = end;
        }
        else
            break;
    }
}

std::string& Config::get_host() {
    return (host);
}

std::vector<unsigned short>& Config::get_port() {
    return (ports);
}

void Config::Print() const {
    std::cout << "Host: " << host << ".\n";
    std::cout << "ServerName: " << server_name << ".\n";
    std::cout << "Root: " << root << ".\n";
    std::cout << "Index: " << index << ".\n";
    std::cout << "Max_body_length: " << max_body_length << ".\n";
    std::cout << "Error pages:\n";
    for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); it++)
        std::cout << "\t" << it->first << ":" << it->second << ".\n";
    std::cout << "Ports: " << host << ".\n";
    for (std::vector<unsigned short>::const_iterator it = ports.begin(); it != ports.end(); it++)
        std::cout << "\t" << *it << "\n";
    std::cout << "Location count: " << locations.size() << ".\n";
}