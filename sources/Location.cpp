#include "Location.hpp"

bool Location::HasLimitedMethods() const {
	return (methods.size() > 0);
}

bool Location::HasMethod(HTTP::Method meth) const {
	return (std::find(methods.begin(), methods.end(), meth) != methods.end());
}

std::string Location::find_word(std::string word)
{
    std::string res;
    size_t start = location_block.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = location_block.find(";", start);
        if (end != std::string::npos)
            res = location_block.substr(start, end - start);
        else
            return "";
    }
    else
        return "";
    return res;
}

std::string Location::find_path(std::string word)
{
    std::string res;
    size_t start = location_block.find(word);
    if (start != std::string::npos)
    {
        start += word.length();
        size_t end = location_block.find(" {", start);
        if (end != std::string::npos)
            res = location_block.substr(start, end - start);
        else
            return "";
    }
    else
        return "";
    return res;
}

Location::Location(std::string& src): location_block(src)
{
    path = this->find_path("location ");
    root = this->find_word("root ");
    index = this->find_word("index ");
    std::string method = find_word("methods ");
    std::stringstream ss(method);
    while (getline(ss, method, ' '))
        methods.push_back(HTTP::EvaluateMethod(method));
}

Location::Location(const Location& other): location_block(other.location_block) {}

Location &Location::operator=(const Location& other)
{
    if (this != &other)
        location_block = other.location_block;
    return *this;
}

Location::~Location() {}
