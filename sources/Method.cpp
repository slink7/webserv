/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 09:51:47 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/05 09:52:03 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Method.hpp"

HTTP::Method HTTP::EvaluateMethod(const std::string &str)
{
	static const int method_count = 3;
	static const std::string methods[method_count] = {
		"GET",
		"POST",
		"DELETE"
	};

	for (int k = 0; k < method_count; k++)
		if (!str.compare(0, methods[k].size(), methods[k]))
			return Method(k);

	return (INVALID);
}