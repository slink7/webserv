/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:23:54 by ellehmim          #+#    #+#             */
/*   Updated: 2024/12/24 20:25:42 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include "Location.hpp"

class Location;

class Server
{
    private :
        std::string _content;
        int _port;
        std::string _host;
        std::string _server_name;
        std::string _root;
        std::string _index;
        std::string _error404;
        std::string _error500;
        std::vector<Location*> location;
    public :
        Server();
        Server(std::string content);
        ~Server();
        void parsing();
        void printconfig() const;
        void count_Block() const;
        void manageLocation(std::string _content);
        int find_wordint(std::string word);
        std::string find_word(std::string word);
    class error_Block : public std::exception
    {
      public :
        virtual const char* what() const throw();
    };
    class error_Value : public std::exception
    {
      public :
        virtual const char* what() const throw();
    };
};

#endif