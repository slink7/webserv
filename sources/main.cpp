

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
		l.root = "var/www/test/";
		l.path = "test/";

		c.locations.push_back(l);

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
	
}