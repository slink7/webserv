#include "CGIHandler.hpp"

bool	CGIHandler::handle(const HTTP::Request& req, int fd) const {
	std::string path = req.GetTarget().substr(1);
	std::map<std::string, std::string>::const_iterator it = get_iterator(path);
	if (it == cgis.end()) {
		Log::out(Log::DEBUG) << path << " is not handled by CGIs\n";
		return (false);
	} else {
		Log::out(Log::DEBUG) << path << " is handled by CGIs\n";
	}

	int ends[2] = {0};
	int s = pipe(ends);
	if (s == -1) {
		Log::out(Log::FUNCTION) << "pipe() failed:" << strerror(errno) << "\n";
		return (false);
	}

	int pid = fork();
	if (pid == -1) {
		Log::out(Log::FUNCTION) << "fork() failed:" << strerror(errno) << "\n";
		return (false);
	}

	if (!pid) {

		// Obtenez le nom de la commande à partir du chemin
		std::string cmd_str = it->second.substr(it->second.find_last_of("/") + 1);
		char cmd[cmd_str.size() + 1];
		std::strcpy(cmd, cmd_str.c_str());

		// Préparez les arguments
		std::vector<char> path_buf(path.size() + 1);
		std::strcpy(path_buf.data(), path.c_str());
		char *const e[] = {cmd, path_buf.data(), 0};

		const std::map<std::string, std::string> &headers = req.GetHeaderMap();

		// Stockage des chaînes pour garantir leur durée de vie
		std::vector<std::string> env_storage;
		std::vector<const char*> envp;

		for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
			// Concaténer la clé et la valeur
			env_storage.push_back(it->first + "=" + it->second);
			// Ajouter un pointeur vers les données de la chaîne
			envp.push_back(env_storage.back().c_str());
		}

		// Terminer le tableau avec un pointeur nul
		envp.push_back(0);

		// Rediriger la sortie

		// Exécuter la commande
		dup2(ends[1], 1);
		execve(it->second.c_str(), e, const_cast<char* const*>(envp.data()));

		// Gestion des erreurs
		Log::out(Log::FUNCTION) << "execve() failed: " << strerror(errno) << "\n";
		exit(420);
	} else {
		int status = waitpid(pid, 0, WUNTRACED);
		if (status == -1)
			Log::out(Log::FUNCTION) << "waitpid() failed: " << strerror(errno) << "\n";
		Log::out(Log::DEBUG) << "Execve exited with code " << status << "\n"; 

		HTTP::Response rep;
		rep.ReadCGI(ends[0]);
		close(ends[0]);

		rep.AddHeader("Content-Length", FT::itoa(rep.GetBody().size()));
		if (rep.GetStartLine().size() < 1) {
			rep.SetStatus("200 OK");
		}

		rep.Print(HTTP::Message::ALL);

		rep.Send(fd);
		//FT::send(fd, res);
	}
	return (true);
}

void	CGIHandler::add(const std::string& extension, const std::string& executable_path) {
	cgis[extension] = executable_path;
}

std::map<std::string, std::string>::const_iterator	CGIHandler::get_iterator(const std::string& path) const {
	std::size_t	index = path.find_last_of('.');
	if (index == std::string::npos)
		return (cgis.end());
	std::string ext = path.substr(index);
	std::map<std::string, std::string>::const_iterator it = cgis.find(ext);
	return (it);
}