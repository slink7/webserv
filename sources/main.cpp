

#include "Log.hpp"
#include "Proxy.hpp"
#include "ConfigGroup.hpp"

int	main() {
	HTTP::Response::InitErrorList();
	
	// Log::SetLevel((Log::Level)0);

	try {
		ConfigGroup cg;

		cg.port.push_back(8080);

		Proxy p;
		p.AddGroup(cg);
		p.AddFD(0, POLLIN);
		p.Run();
	} catch (std::exception& e) {
		Log::out(Log::ERROR) << "Exception: " << e.what() << "\n";
	}
	
}