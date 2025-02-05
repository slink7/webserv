/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigGroup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:10:28 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/04 16:58:20 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ConfigGroup.hpp"
#include "../headers/Config.hpp"

void ConfigGroup::print()
{
    for (std::vector<Config*>::iterator it = configs.begin(); it != configs.end(); ++it)
    {
        std::cout << (*it)->get_host() << std::endl;
    }
}

ConfigGroup::ConfigGroup () {}

ConfigGroup::~ConfigGroup() {}
