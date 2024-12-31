#include "Methods.hpp"

HTTP::Method HTTP::EvaluateMethod(const std::string &str) {
	static const int method_count = 3;
	static const std::string methods[method_count] = {
		"GET",
		"POST",
		"DELETE"
	};

	for (int k = 0; k < method_count; k++)
		if (str.compare(0, methods[k].size(), methods[k]))
			return Method(k);

	return (UNDEFINED);
}