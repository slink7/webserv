/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:06:03 by ellehmim          #+#    #+#             */
/*   Updated: 2025/02/02 10:06:04 by ellehmim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error {
public:
	int			code;
	std::string	status;
	std::string	default_path;
	std::string	custom_path;

	Error();
	Error(int code, const std::string& status, const std::string& default_path, const std::string& custom_path);
	private:
};

#endif