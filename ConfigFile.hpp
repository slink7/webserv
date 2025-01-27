#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <string>
#include <map>
#include <vector>

// #include "Config.hpp"

struct Location {
    std::string					path;                 // Chemin de la location
    std::string					root;                 // Racine pour cette location
    std::string					index;                // Page d'index
    std::vector<std::string>	methods; // Méthodes HTTP autorisées
};

class Config {
	std::vector<int>			ports;
	std::string					host;
	std::string					server_name;
	std::string					root;
	std::string					index;
	unsigned long				max_body_length;
	std::map<int, std::string>	error_pages;
	std::vector<Location>		locations;
};

class ConfigFile {

public:

	ConfigFile(const std::string& path);
	
	bool	LoadFromFile(const std::string& path);

private:
	std::map<unsigned short, std::vector<Config>> configs;
};

#endif