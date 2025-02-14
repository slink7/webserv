#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "FT.hpp"
#include "Method.hpp"

struct Location
{
    private:
    public:
        std::string                 location_block;
        std::string					path;
        std::string					root;
        std::string					index;
        std::vector<HTTP::Method>	methods;
        Location(std::string& src);
        ~Location();
        std::string find_word(std::string word);
        std::string find_path(std::string word);
        bool        HasLimitedMethods() const;
        bool        HasMethod(HTTP::Method meth) const; 
};

#endif