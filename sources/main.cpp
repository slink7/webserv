<<<<<<< HEAD


#include "Log.hpp"
#include "Proxy.hpp"
#include "ConfigGroup.hpp"

int	main() {
	HTTP::Response::InitErrorList();
	
	// Log::SetLevel((Log::Level)0);

	// try {
		ConfigGroup cg;

		cg.port.push_back(8080);
		
		Config c;
		c.cgi.Add(".php", "/usr/bin/php-cgi");
		c.server_name = "swebserv";
		c.index = "index.html";
		c.root = "var/www/";
		c.host = "127.0.0.1";

		Location l;
		l.methods.push_back(HTTP::GET);
		l.index = "test.html";
		l.root = "var/www/GETtest/";
		l.path = "test/";

		c.locations.push_back(l);

        Location m;
		m.methods.push_back(HTTP::POST);
		m.index = "test.html";
		m.root = "var/www/POSTtest/";
		m.path = "test/";

		c.locations.push_back(m);

        Location n;
		n.methods.push_back(HTTP::POST);
		n.index = "test.html";
		n.root = "var/www/noget/";
		n.path = "noget/";

		c.locations.push_back(n);

        

		std::cout << "Adding group\n";
		cg.configs.push_back(c);
		
		HTTP::Request req = HTTP::Request();
		cg.GetConfig(req);

		Proxy p;
		p.AddGroup(cg);
		p.AddFD(0, POLLIN);
		p.Run();
	// } catch (std::exception& e) {
	// 	Log::out(Log::ERROR) << "Exception: " << e.what() << "\n";
	// }
	
=======
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
>>>>>>> parsing
}