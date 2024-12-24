/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:23:01 by ellehmim          #+#    #+#             */
/*   Updated: 2024/12/24 20:38:58 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
 #include <fstream>
#include <string>
#include "server.hpp"

std::string readFileToString(const std::string& filePath)
{
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary); 
    if (!file)
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filePath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

int multiserv(std::string content)
{
    int c = 0;
    std::string word = "server ";
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
        return 0;
    return c;
}

std::string *splitserv(std::string &content)
{
    int c = multiserv(content);
    int i = 0;
    size_t start = 0;
    std::string* tab = new std::string[c];
    std::string word = "server ";
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

int main()
{
    try
    {
        std::string content = readFileToString("server.conf");
        int t = multiserv(content);
        if (t > 1)
        {
            std::string *tab = splitserv(content);
            Server* horde = new Server[t];
            for(int i = 0; i < t; i++)
            {
                horde[i] = Server(tab[i]);
                horde[i].printconfig();
                horde[i].parsing();
            }
            delete[] tab;
            delete[] horde;
        }
        else if (t == 1)
        {
            Server server(content);
            server.printconfig();
            server.parsing();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
    return 0;
}
/*
    -L'utilisation de istreambuf_iterator permet de lire rapidement et efficacement tout un fichier, même s'il est volumineux.
    -std::ifstream file(filePath, std::ios::in | std::ios::binary);
        Ouvre le file en binaire pour eviter toute transformation.
    -std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        Début : std::istreambuf_iterator<char>(file) pointe sur le début des données du flux.
        Fin : std::istreambuf_iterator<char>() est un itérateur spécial "fin de flux", qui marque la fin du fichier.
        Ces itérateurs permettent de lire tout le contenu du fichier en une seule ligne.
*/