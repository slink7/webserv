#include "Socket.hpp"

int	main() {
	Socket sock(8080, 5);

	if (!sock.is_enabled())
		return (1);

	char buffer[1024] = {0};
	while (strncmp(buffer, "STOP", 4)) {
		struct sockaddr addr = {0};
		int client = accept(sock.get_fd(), 0, 0);

		if (recv(client, buffer, sizeof(buffer) - 1, 0) < 0) {
			std::cerr << "recv() failed: " << strerror(errno) << "\n";
			continue ;
		}

		std::cout << "Received: \n" << buffer << "\n";
		close(client);
	}
	return (0);
}