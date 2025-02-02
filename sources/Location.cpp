/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 06:25:59 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/02 09:59:16 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Location.hpp"

std::string Location::find_word(std::string word)
{
    std::string res;
    size_t start = location_block.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = location_block.find(";", start);
        if (end != std::string::npos)
            res = location_block.substr(start, end - start);
        else
            return "";
    }
    else
        return "";
    return res;
}

std::string Location::find_path(std::string word)
{
    std::string res;
    size_t start = location_block.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = location_block.find(" {", start);
        if (end != std::string::npos)
            res = location_block.substr(start, end - start);
        else
            return "";
    }
    else
        return "";
    return res;
}

Location::Location(std::string& src): location_block(src)
{
    path = this->find_path("location ");
    root = this->find_word("root ");
    index = this->find_word("index ");
}

Location::Location(const Location& other): location_block(other.location_block) {}

Location &Location::operator=(const Location& other)
{
    if (this != &other)
        location_block = other.location_block;
    return *this;
}

Location::~Location() {}