/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigGroup.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:08:11 by ellehmim          #+#    #+#             */
/*   Updated: 2025/01/31 12:08:16 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGGROUP_HPP
#define CONFIGGROUP_HPP

#include <vector>
#include <iostream>
#include <cstdlib>

#include "Config.hpp"

class ConfigGroup
{
	private :
		std::string 				*_tab;
		int 						_size;
		std::vector<Config>			configs;
		std::vector<unsigned short>	port;
	public :
		ConfigGroup(std::string *tab, int c);
		~ConfigGroup();
		// std::string *porthandler(std::string *tab, int size);
};

#endif