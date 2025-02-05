/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:05:28 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/05 09:51:07 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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