/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 19:45:02 by ellehmim          #+#    #+#             */
/*   Updated: 2024/12/24 20:44:49 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "server.hpp"

class Location
{
    private :
        std::string _content;
        std::string _name;
        std::string _root;
        std::string _index;
    public :
        Location();
        Location(std::string content);
        ~Location();
        void printlocation() const;
        void parsing();
        std::string find_word(std::string word, int i);
};

#endif