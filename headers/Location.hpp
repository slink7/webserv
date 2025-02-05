#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <algorithm>

#include "Method.hpp"

struct Location {
    std::string					path;
    std::string					root;
    std::string					index;
    std::vector<HTTP::Method>	methods;

    bool    HasLimitedMethods() const;
    bool    HasMethod(HTTP::Method meth) const; 
};

#endif