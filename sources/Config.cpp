/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:04:57 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/04 16:34:11 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Config.hpp"
#include "../headers/Location.hpp"

std::string Config::find_word(std::string word)
{
    std::string res;
    size_t start = _src.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = _src.find(";", start);
        if (end != std::string::npos)
            res = _src.substr(start, end - start);
        else
            return "";
    }
    else
        return "";
    return res;
}

unsigned long Config::find_wordint(std::string word)
{
    std::string res;
    size_t start = _src.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = _src.find(";", start);
        if (end != std::string::npos)
            res = _src.substr(start, end - start);
        else
            return -1;
    }
    else
        return -1;
    return static_cast<unsigned long>(std::atoi(res.c_str()));
}

void Config::manageLocation(std::string _content)
{
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

std::string Config::get_host()
{
    return this->host;
}

std::vector<unsigned short>& Config::get_port()
{
    return this->ports;
}

std::vector<unsigned short> porthandler(const std::string& src)
{
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
            ports.push_back(std::atoi(buff.c_str()));
        }
        else
            {
                buff = src.substr(start);
                ports.push_back(std::atoi(buff.c_str()));
                break; 
            } 
        start = src.find(word, end);
    }
    return ports;
}

Config::Config(std::string& src): _src(src)
{
    ports = porthandler(_src);
    host = this->find_word("host ");
    server_name = this->find_word("server_name ");
    root = this->find_word("root ");
    index = this->find_word("index ");
    max_body_length = this->find_wordint("client_max_body_size ");
    std::string word = "error_page ";
    std::string buf;
    std::string root_error;
    std::string errornb;
    size_t start = _src.find(word);
    while (start != std::string::npos)
    {
        start += word.length();
        size_t end = _src.find(";", start);
        if (end != std::string::npos)
        {
            buf = _src.substr(start, end - start);
            std::stringstream ss(buf);
            getline(ss, errornb, ' ') && (ss >> root_error);
            error_pages[static_cast<int>(std::atoi(errornb.c_str()))] = root_error;
            start = _src.find(word, end);
        }
    }
    manageLocation(_src);
}

Config::Config () {}
Config::~Config () {}