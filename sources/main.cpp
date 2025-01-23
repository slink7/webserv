

#include "Log.hpp"
#include "Proxy.hpp"

int	main() {
	HTTP::Response::InitErrorList();
	
	// Log::SetLevel((Log::Level)0);

	try {
		Proxy p;
		p.AddServer((Config){8080});
		p.AddFD(0, POLLIN);
		p.Run();
	} catch (std::exception& e) {
		Log::out(Log::ERROR) << "Exception: " << e.what() << "\n";
	}
	
}