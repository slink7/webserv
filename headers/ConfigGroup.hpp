/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigGroup.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:08:11 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/04 17:00:24 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGGROUP_HPP
#define CONFIGGROUP_HPP

#include <vector>
#include <iostream>
#include <cstdlib>

#include "Config.hpp"

class ConfigGroup;

class ConfigGroup
{
	public :
		std::vector<Config*>			configs;
		ConfigGroup();
		~ConfigGroup();
		void print();
};

#endif