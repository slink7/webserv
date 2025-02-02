/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigGroup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:10:28 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/02 11:24:27 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ConfigGroup.hpp"
#include "../headers/Config.hpp"


ConfigGroup::~ConfigGroup() {}

int portnum(std::string tab)
{
    int c = 0;
    std::string word = "listen ";
    size_t pos = tab.find(word);
    if (pos != std::string::npos)
    {
        while (pos != std::string::npos)
        {
            c++;
            pos = tab.find(word, pos + word.length());
        }
    }
    else
        return 0;
    return c;
}

std::string *porthandler(std::string *tab, int size)
{
    int c = 0;
    int k = 0;
    std::string word = "listen ";
    for (int i = 0; i < size; i++)
        c += portnum(tab[i]);
    if (c == 0)
        throw std::runtime_error("Error: no ports.");
    std::string* ports = new std::string[c];
    for (int i = 0; i < size && k < c; i++)
    {
        if(!tab[i].empty())
        {
            size_t start = tab[i].find(word);
            while (start != std::string::npos  && k < c)
            {
                start += word.length();
                size_t end = tab[i].find(";", start);
                if (end != std::string::npos)
                {
                    ports[k] = tab[i].substr(start, end - start);
                    k++;
                }
                start = tab[i].find(word, end);
            }
        }
    } 
    return ports;
}

ConfigGroup::ConfigGroup (std::string *tab, int c): _tab(tab), _size(c)
{
    int nb = 0;
    std::string *ports = porthandler(_tab, _size);
    for (int i = 0; i < _size; i++)
        nb += portnum(_tab[i]);
    for(int i = 0; i < nb; i++)
    {
        if(std::atoi(ports[i].c_str()) > 65536 || std::atoi(ports[i].c_str()) < 0)
        {
            delete [] ports;
            delete [] _tab;
            throw std::runtime_error("the port must be between 0 and 65539");
        }
        port.push_back(std::atoi(ports[i].c_str()));
    }
    for(size_t i = 0; i < port.size(); i++)
    {
        for (size_t j = i + 1; j < port.size(); j++)
            if (port[i] == port[j])
                port.erase(port.begin() + j);
    }
    for(int i = 0; i < c; i++)
    {
        configs.push_back(Config(tab[i]));
    }
    delete[] ports;
}