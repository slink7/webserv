#include "CGIHandler.hpp"

bool	CGIHandler::handle(const std::string& path) const {
	std::map<std::string, std::string>::const_iterator it = get_iterator(path);
	if (it == cgis.end())
		return (false);

	int ends[2] = {0};
	int s = pipe(ends);
	if (s == -1) {
		std::cerr << "pipe() failed:" << strerror(errno) << "\n";
		return (false);
	}

	int pid = fork();
	if (pid == -1) {
		std::cerr << "fork() failed:" << strerror(errno) << "\n";
		return (false);
	}
	if (!pid) {
		char *const cmd = (char *const) it->second.substr(it->second.find_last_of("/")).c_str();
		char *const e[] = {
			cmd, (char *const)path.c_str(), 0
		};
		char *const f[] = {
			"HOST=127.360.420.1:8080", 0
		};
		dup2(ends[1], 1);
		execve(it->second.c_str(), e, f);
		std::cerr << "execve() failed: " << strerror(errno) << "\n";
	} else {
		int status = waitpid(pid, 0, WUNTRACED);
		if (status == -1)
			std::cerr << "waitpid() failed: " << strerror(errno) << "\n";
		std::cout << "execve exited with code " << status << "\n"; 
		std::string res;
		msg::read(ends[0], res);
		std::cout << "execve returned " << res.size() << " bytes\n";
	}
	return (true);
}

void	CGIHandler::add(const std::string& extension, const std::string& executable_path) {
	cgis.insert({extension, executable_path});
}

std::map<std::string, std::string>::const_iterator	CGIHandler::get_iterator(const std::string& path) const {
	int	index = path.find_last_of('.');
	if (index == std::string::npos)
		return (cgis.end());
	std::string ext = path.substr(index);
	std::map<std::string, std::string>::const_iterator it = cgis.find(ext);
	return (it);
}