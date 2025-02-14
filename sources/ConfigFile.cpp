
#include "../headers/ConfigFile.hpp"
#include "../headers/ConfigGroup.hpp"
#include "../headers/Config.hpp"

ConfigFile::ConfigFile(const std::string& path) {
    LoadFromFile(path);
}

ConfigFile::~ConfigFile() {}

std::string *splitserv(std::string &content, int c)
{
    int i = 0;
    std::string* tab = new std::string[c];
    std::string word = "server ";
    size_t start = content.find(word, 0);
    while (i < c) 
    {
        size_t end = content.find(word, start + word.length());
        if (end != std::string::npos)
            tab[i] = content.substr(start, end - start);
        else
        {
            tab[i] = content.substr(start);
            break;
        }
        start = end;
        i++;
    }
    return tab;
}

void    check_config(std::string content)
{
    int c = 0;
    int cc = 0;
    std::string word = "{";
    size_t pos = content.find(word);
    if (pos != std::string::npos)
    {
        while (pos != std::string::npos)
        {
            c++;
            pos = content.find(word, pos + word.length());
        }
    }
    else
        throw std::runtime_error("block architecture not respected");
    word = "}";
    pos = content.find(word);
    if (pos != std::string::npos)
    {
        while (pos != std::string::npos)
        {
            cc++;
            pos = content.find(word, pos + word.length());
        }
    }
    else
        throw std::runtime_error("block architecture not respected");
    if (c != cc)
        throw std::runtime_error("block architecture not respected");
}

void ConfigFile::startconfig(std::string content)
{
    check_config(content);
    int c = FT::count(content, "server ");
    if (c == 0)
        throw std::runtime_error("no Server block in config file");
    std::string *tab = splitserv(content, c);
    std::string word = "listen ";
    std::string buff;
    for(int i = 0; i < c; i++)
        config_list.push_back(Config(tab[i]));
    delete[] tab;
    for (std::vector<Config>::iterator it = config_list.begin(); it != config_list.end(); ++it)
    {
        std::vector <unsigned short>& temp = it->get_port();
        for (std::vector<unsigned short>::iterator at = temp.begin(); at != temp.end(); ++at)
                port_config[*at].configs.push_back(&(*it));
    }
}

void ConfigFile::LoadFromFile(const std::string &path) {
    std::string raw;

    FT::get_file(raw, path);
    startconfig(raw);
}