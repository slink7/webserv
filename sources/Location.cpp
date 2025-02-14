#include "Location.hpp"

bool Location::HasLimitedMethods() const {
	return (methods.size() > 0);
}

bool Location::HasMethod(HTTP::Method meth) const {
	return (std::find(methods.begin(), methods.end(), meth) != methods.end());
}

Location::Location(std::string& src): location_block(src)
{
    path = FT::get_value(src, "location ", "{");
    root = FT::get_value(src, "root ", ";");
    index = FT::get_value(src, "index ", ";");
    std::string method = FT::get_value(src, "methods ", ";");
    std::stringstream ss(method);
    while (getline(ss, method, ' '))
        methods.push_back(HTTP::EvaluateMethod(method));
}

Location::~Location() {}
