#include "Location.hpp"

bool Location::HasLimitedMethods() const {
	return (methods.size() > 0);
}

bool Location::HasMethod(HTTP::Method meth) const {
	return (std::find(methods.begin(), methods.end(), meth) != methods.end());
}
