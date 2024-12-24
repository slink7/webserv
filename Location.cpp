/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 19:51:09 by ellehmim          #+#    #+#             */
/*   Updated: 2024/12/24 20:50:05 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include <iostream>

Location::Location() : _content("") {}

Location::Location(std::string content) : _content(content)
{
    parsing();
}

Location::~Location() {}

void Location::printlocation() const
{
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << _content << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
}

std::string Location::find_word(std::string word, int i)
{
    std::string res;
    size_t start = _content.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = _content.find(";", start);
        if (i == 1)
            end = _content.find(" {", start);
        if (end != std::string::npos)
            res = _content.substr(start, end - start);
        else
            return "";
    }
    else
        return "";
    return res;
}

void Location::parsing()
{
    _name = this->find_word("location /", 1);
    _root = this->find_word("root ", 0);
    _index = this->find_word("index ", 0);
    std::cout << _name << std::endl;
    std::cout << _root << std::endl;
    std::cout << _index << std::endl;
}