#ifndef METHOD_HPP
#define METHOD_HPP

#include <string>

namespace HTTP {
	enum Method {
		UNDEFINED = -2,
		INVALID = -1,
		GET = 0,
		POST = 1,
		DELETE = 2
	};

	Method	EvaluateMethod(const std::string& str);
}

#endif