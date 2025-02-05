/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:49:38 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/04 16:57:39 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ConfigFile.hpp"
#include "../headers/ConfigGroup.hpp"
#include "../headers/Config.hpp"

ConfigFile::ConfigFile()
{
    this-> _content = readFileToString("../configs/test.conf");
    startconfig(_content);
}

ConfigFile::~ConfigFile() {}

std::string ConfigFile::readFileToString(const std::string& filePath)
{
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary); 
    if (!file)
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filePath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

int multiserv(std::string content)
{
    int c = 0;
    std::string word = "server ";
    size_t pos = content.find(word);
    while (pos != std::string::npos)
    {
        c++;
        pos = content.find(word, pos + word.length());
    }
    return c;
}

std::string *splitserv(std::string &content, int c)
{
    int i = 0;
    std::string* tab = new std::string[c];
    std::string word = "server ";
    size_t start = content.find(word, 0);
    while (i < c) 
    {
        size_t end = content.find(word, start + word.length());
        if (end != std::string::npos)
            tab[i] = content.substr(start, end - start);
        else
        {
            tab[i] = content.substr(start);
            break;
        }
        start = end;
        i++;
    }
    return tab;
}

void    check_config(std::string _content)
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
        throw std::runtime_error("block architecture not respected");
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
        throw std::runtime_error("block architecture not respected");
    if (c != cc)
        throw std::runtime_error("block architecture not respected");
}

void ConfigFile::startconfig(std::string _content)
{
    check_config(_content);
    int c = multiserv(_content);
    if (c == 0)
        throw std::runtime_error("no Server block in config file");
    std::string *tab = splitserv(_content, c);
    std::string word = "listen ";
    std::string buff;
    for(int i = 0; i < c; i++)
        config_list.push_back(Config(tab[i]));
    for (std::vector<Config>::iterator it = config_list.begin(); it != config_list.end(); ++it)
    {
        std::vector <unsigned short>& temp = it->get_port();
        for (std::vector<unsigned short>::iterator at = temp.begin(); at != temp.end(); ++at)
            {
                port_config[*at].configs.push_back(&(*it));
            }
    }
    for (std::map<unsigned short, ConfigGroup>::iterator it = port_config.begin(); it != port_config.end(); ++it)
    {
        std::cout << it->first << std::endl;
        it->second.print();
    }
    delete[] tab; 
}