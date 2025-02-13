/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:42:59 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/13 15:53:34 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ConfigFile.hpp"
#include <iostream>


int main()
{
    try {
        ConfigFile config("../configs/test.conf");
        for (std::vector<Config>::const_iterator it = config.config_list.begin(); it != config.config_list.end(); it++) {
            std::cout << "\n\t\tConfig\n";
            it->Print();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
    return 0;
}