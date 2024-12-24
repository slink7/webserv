/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:50:38 by ellehmim          #+#    #+#             */
/*   Updated: 2024/12/24 20:49:45 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : _content("") {}

Server::Server(std::string content) : _content(content) {}

Server::~Server()
{
    for (size_t i = 0; i < location.size(); ++i)
        delete location[i];
}

void Server::printconfig() const
{
    std::cout << _content << std::endl;
}

void    Server::count_Block() const
{
    int c = 0;
    int cc = 0;
    std::string word = "{";
    size_t pos = _content.find(word);
    if (pos != std::string::npos)
    {
        while (pos != std::string::npos)
        {
            c++;
            pos = _content.find(word, pos + word.length());
        }
    }
    else
        throw Server::error_Block();
    word = "}";
    pos = _content.find(word);
    if (pos != std::string::npos)
    {
        while (pos != std::string::npos)
        {
            cc++;
            pos = _content.find(word, pos + word.length());
        }
    }
    else
        throw Server::error_Block();
    if (c != cc)
        throw Server::error_Block();
}

std::string Server::find_word(std::string word)
{
    std::string res;
    size_t start = _content.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = _content.find(";", start);
        if (end != std::string::npos)
            res = _content.substr(start, end - start);
        else
            return "";
    }
    else
        return "";
    return res;
}

int Server::find_wordint(std::string word)
{
    std::string res;
    size_t start = _content.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = _content.find(";", start);
        if (end != std::string::npos)
            res = _content.substr(start, end - start);
        else
            return -1;
    }
    else
        return -1;
    if(std::atoi(res.c_str()) > 65536 || std::atoi(res.c_str()) < 0)
        return -1;
    return std::atoi(res.c_str());
}

void Server::manageLocation(std::string _content)
{
    size_t start = 0;
    std::string location_start = "location ";
    std::string location_end = "}";
    while ((start = _content.find(location_start, start)) != std::string::npos)
    {
        size_t end = _content.find(location_end, start);
        if (end != std::string::npos)
        {
            std::string location_block = _content.substr(start, end - start + 1);
            location.push_back(new Location(location_block));
            start = end + 1;
        }
    }
}

void Server::parsing()
{
    count_Block();
    _port = this->find_wordint("listen ");
    _host = this->find_word("host ");
    _root = this->find_word("root ");
    _index = this->find_word("index ");
    _error404 = this->find_word("error_page 404 ");
    _error500 = this->find_word("error_page 500 ");
    if (_content.find("error_page 500 ") != std::string::npos)
        _server_name = this->find_word("error_page 500 ");
    if (_content.find("server_name ") != std::string::npos)
        _server_name = this->find_word("server_name ");
    if (_port < 0 || _host.empty() || _root.empty() || _index.empty() || _error404.empty())
        throw Server::error_Value();
    std::cout << _port << std::endl;
    std::cout << _host << std::endl;
    std::cout << _root << std::endl;
    std::cout << _index << std::endl;
    std::cout << _error404 << std::endl;
    std::cout << _error500 << std::endl;
    manageLocation(_content);
}

const char* Server::error_Block::what() const throw()
{
    return ("error Block in file config please check");
}

const char* Server::error_Value::what() const throw()
{
    return ("error Value in file config please check");
}
