

#include "Proxy.hpp"

int	main() {
	HTTP::Response::InitErrorList();
	
	// Log::SetLevel((Log::Level)0);

	Proxy p;
	p.AddServer((Config){8080});
	p.AddFD(0, POLLIN);
	p.Run();
}