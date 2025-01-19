

#include "Proxy.hpp"

int	main() {
	HTTP::Response::InitErrorList();
	
	Proxy p;
	p.AddServer((Config){8080});
	p.Run();
}