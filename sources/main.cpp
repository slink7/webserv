

#include "Log.hpp"
#include "Proxy.hpp"
#include "ConfigGroup.hpp"
#include "ConfigFile.hpp"

int	main() {
	HTTP::Response::InitErrorList();
	
	

    ConfigFile file("configs/base.conf");

	for (std::vector<Location>::iterator it = file.config_list[0].locations.begin(); it != file.config_list[0].locations.end(); it++) {
		std::cout << "Path: " << it->path << "\nRoot: " << it->root << "\nIndex: " << it->index << "\n";
	}

	Proxy p;
	p.AddGroup(file);
	p.AddFD(0, POLLIN);
	p.Run();
	
}